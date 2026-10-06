// Terminal supervisor: one WebSocket = one sandboxed bash session.
// Runs as root only to create a throw-away Unix user per session; the shell itself is
// started through /usr/local/sbin/sandbox-launch, which sets limits and drops root.
import { WebSocketServer } from 'ws';
import http from 'node:http';
import fs from 'node:fs';
import path from 'node:path';
import { execFileSync } from 'node:child_process';
import pty from 'node-pty';

const PORT = parseInt(process.env.PORT || '8080');
const MAX_SESSIONS = parseInt(process.env.MAX_SESSIONS || '6');
const IDLE_TIMEOUT_MS = parseInt(process.env.SESSION_TIMEOUT || '180000');
const MAX_SESSION_MS = parseInt(process.env.MAX_SESSION_MS || String(20 * 60 * 1000));
const CONTAINER_NAME = process.env.CONTAINER_NAME || 'local';

const LAUNCHER = '/usr/local/sbin/sandbox-launch';
const WORK_ROOT = '/work';
const MAX_FILES = 60;
const MAX_TOTAL_BYTES = 1_000_000;
const MAX_INPUT_CHARS = 64 * 1024;
const SAFE_SEGMENT = /^[A-Za-z0-9_-][A-Za-z0-9._-]*$/; // no leading dot, no '..'
// The terminal turns these control characters into signals for the foreground process group
const SIGNAL_KEYS = { SIGINT: '\x03', SIGQUIT: '\x1c', SIGTSTP: '\x1a' };

const sessions = new Map();
let totalSessionsServed = 0;
const startedAt = Date.now();

const log = (id, msg) => console.log(`[${new Date().toISOString()}] [${id || '--------'}] ${msg}`);
const run = (cmd, args) => execFileSync(cmd, args, { stdio: ['ignore', 'pipe', 'ignore'] }).toString().trim();

fs.mkdirSync(WORK_ROOT, { recursive: true, mode: 0o711 });

// "dir/name.c" -> kept as a relative path; anything absolute, hidden or with '..' -> null
function safeRelativePath(name) {
  if (typeof name !== 'string' || !name || name.length > 200) return null;
  const parts = name.split('/');
  return parts.every((p) => SAFE_SEGMENT.test(p)) ? parts.join('/') : null;
}

function createSessionUser(sid) {
  const user = `cs${sid}`;
  const dir = path.join(WORK_ROOT, sid);
  run('adduser', ['-D', '-H', '-h', dir, '-s', '/bin/bash', '-G', 'students', user]);
  return { user, dir, uid: Number(run('id', ['-u', user])), gid: Number(run('id', ['-g', user])) };
}

// System V message queues, shared memory and semaphores outlive their processes (Lesson 09/10).
// Removed *as their owner*: container root lacks CAP_IPC_OWNER, but an owner may always IPC_RMID.
function removeIpcObjects(uid, gid) {
  for (const [file, idCol, flag] of [['msg', 'msqid', '-q'], ['shm', 'shmid', '-m'], ['sem', 'semid', '-s']]) {
    let rows;
    try { rows = fs.readFileSync(`/proc/sysvipc/${file}`, 'utf8').trim().split('\n'); } catch { continue; }
    const header = rows.shift().trim().split(/\s+/);
    const idIdx = header.indexOf(idCol);
    const uidIdx = header.indexOf('uid');
    for (const row of rows) {
      const cols = row.trim().split(/\s+/);
      if (Number(cols[uidIdx]) === uid) {
        try { execFileSync('ipcrm', [flag, cols[idIdx]], { uid, gid, stdio: 'ignore' }); } catch {}
      }
    }
  }
}

function destroySessionUser({ user, uid, gid, dir }) {
  // Kill everything the user owns; repeat because a fork bomb may race the first kill
  for (let i = 0; i < 10; i++) {
    try { run('pkill', ['-KILL', '-u', user]); } catch {}
    try { run('pgrep', ['-u', user]); } catch { break; } // pgrep exits 1 when none are left
  }
  removeIpcObjects(uid, gid);
  fs.rmSync(dir, { recursive: true, force: true });
  try { run('deluser', [user]); } catch {}
}

const server = http.createServer((req, res) => {
  res.setHeader('Access-Control-Allow-Origin', '*');
  if (req.url === '/health') {
    res.writeHead(200, { 'Content-Type': 'application/json' });
    res.end(JSON.stringify({
      status: 'ok',
      container: CONTAINER_NAME,
      activeSessions: sessions.size,
      maxSessions: MAX_SESSIONS,
      totalServed: totalSessionsServed,
      sessionTimeoutMs: IDLE_TIMEOUT_MS,
      maxSessionMs: MAX_SESSION_MS,
      uptimeSeconds: Math.floor((Date.now() - startedAt) / 1000),
    }));
    return;
  }
  res.writeHead(404);
  res.end();
});

const wss = new WebSocketServer({ server, maxPayload: 2 * 1024 * 1024 });

wss.on('connection', (ws) => {
  const send = (msg) => { if (ws.readyState === 1) ws.send(JSON.stringify(msg)); };
  const refuse = (text) => { send({ type: 'error', data: `${text}\r\n` }); ws.close(); };

  if (sessions.size >= MAX_SESSIONS) return refuse(`Server at capacity (${MAX_SESSIONS} sessions). Try again in a minute.`);

  let sid = null;
  let account = null;
  let ptyProcess = null;
  let idleTimer = null;
  let hardTimer = null;
  let closed = false;

  function cleanup(reason) {
    if (closed) return;
    closed = true;
    clearTimeout(idleTimer);
    clearTimeout(hardTimer);
    if (ptyProcess) { try { ptyProcess.kill('SIGKILL'); } catch {} }
    if (account) { try { destroySessionUser(account); } catch (e) { log(sid, `cleanup error: ${e.message}`); } }
    if (sid) { sessions.delete(sid); log(sid, `ended (${reason}); ${sessions.size} active`); }
    try { ws.close(); } catch {}
  }

  function resetIdle() {
    clearTimeout(idleTimer);
    idleTimer = setTimeout(() => {
      send({ type: 'output', data: '\r\n\x1b[33m[Session closed after 3 minutes without activity]\x1b[0m\r\n' });
      cleanup('idle');
    }, IDLE_TIMEOUT_MS);
  }

  ws.on('message', (raw) => {
    let msg;
    try { msg = JSON.parse(raw.toString()); } catch { return; }
    resetIdle();

    if (msg.type === 'start') {
      if (ptyProcess || closed) return;

      const files = Array.isArray(msg.files) ? msg.files : [];
      const totalBytes = files.reduce((n, f) => n + (typeof f?.content === 'string' ? f.content.length : 0), 0);
      if (files.length > MAX_FILES || totalBytes > MAX_TOTAL_BYTES) {
        return refuse(`Too many files (${files.length}, max ${MAX_FILES}) or too much code (${totalBytes} bytes, max ${MAX_TOTAL_BYTES}).`);
      }
      const named = files.map((f) => ({ rel: safeRelativePath(f?.name), content: String(f?.content ?? '') }));
      const bad = named.find((f) => !f.rel);
      if (bad) return refuse('Invalid file name.');

      sid = crypto.randomUUID().replace(/-/g, '').slice(0, 8);
      try {
        account = createSessionUser(sid);
        fs.mkdirSync(account.dir, { mode: 0o700 });
        for (const f of named) {
          const target = path.join(account.dir, f.rel);
          fs.mkdirSync(path.dirname(target), { recursive: true });
          fs.writeFileSync(target, f.content);
        }
        // Hand the whole tree to the session user; 0700 keeps other students out
        execFileSync('chown', ['-R', `${account.uid}:${account.gid}`, account.dir]);
        fs.chmodSync(account.dir, 0o700);
      } catch (e) {
        log(sid, `setup failed: ${e.message}`);
        send({ type: 'error', data: 'Could not prepare your sandbox. Please try again.\r\n' });
        cleanup('setup failed');
        return;
      }

      const mode = msg.mode === 'lesson' ? 'lesson' : 'example';
      ptyProcess = pty.spawn(LAUNCHER, [String(account.uid), String(account.gid), account.dir, mode], {
        name: 'xterm-256color',
        cols: Math.min(Math.max(Number(msg.cols) || 80, 20), 400),
        rows: Math.min(Math.max(Number(msg.rows) || 24, 5), 200),
        cwd: account.dir,
        env: { PATH: '/usr/bin:/bin' },
      });

      sessions.set(sid, { ws, startedAt: Date.now() });
      totalSessionsServed++;
      log(sid, `started as ${account.user} (uid ${account.uid}) with ${named.length} files, mode=${mode}; ${sessions.size} active`);

      hardTimer = setTimeout(() => {
        send({ type: 'output', data: '\r\n\x1b[33m[Session reached the 20 minute limit — open a new terminal to continue]\x1b[0m\r\n' });
        cleanup('max duration');
      }, MAX_SESSION_MS);

      ptyProcess.onData((data) => send({ type: 'output', data }));
      ptyProcess.onExit(({ exitCode }) => {
        send({ type: 'exit', code: exitCode });
        cleanup('shell exited');
      });
      send({ type: 'ready', sessionId: sid, container: CONTAINER_NAME });

    } else if (msg.type === 'input') {
      if (ptyProcess && typeof msg.data === 'string' && msg.data.length <= MAX_INPUT_CHARS) ptyProcess.write(msg.data);

    } else if (msg.type === 'resize') {
      const cols = Number(msg.cols), rows = Number(msg.rows);
      if (ptyProcess && cols >= 20 && cols <= 400 && rows >= 5 && rows <= 200) {
        try { ptyProcess.resize(cols, rows); } catch {}
      }

    } else if (msg.type === 'signal') {
      // Writing ^C to the terminal makes the kernel deliver SIGINT to the foreground
      // process group — the running program — not to the (signal-ignoring) shell.
      const key = SIGNAL_KEYS[msg.signal];
      if (ptyProcess && key) ptyProcess.write(key);
    }
  });

  ws.on('close', () => cleanup('disconnected'));
  ws.on('error', () => cleanup('socket error'));
  resetIdle();
});

server.listen(PORT, () => log(null, `terminal supervisor on :${PORT} (container=${CONTAINER_NAME}, max ${MAX_SESSIONS} sessions)`));
