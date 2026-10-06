// Terminal supervisor: one WebSocket = one sandboxed session (one Unix user, up to 3 shells "panes").
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
// Bump when the image changes, so validation can tell when a gradual rollout has reached a container
const IMAGE_VERSION = '2026-10-07.1';

const LAUNCHER = '/usr/local/sbin/sandbox-launch';
const WORK_ROOT = '/work';
const MAX_FILES = 60;
const MAX_TOTAL_BYTES = 1_000_000;
const MAX_INPUT_CHARS = 64 * 1024;
const SAFE_SEGMENT = /^[A-Za-z0-9_-][A-Za-z0-9._-]*$/; // no leading dot, no '..'
// The terminal turns these control characters into signals for the foreground process group
const SIGNAL_KEYS = { SIGINT: '\x03', SIGQUIT: '\x1c', SIGTSTP: '\x1a' };
const MAX_PANES = 3;
const INPUT_POLL_MS = 600;

const sessions = new Map();
let totalSessionsServed = 0;
const startedAt = Date.now();

const log = (id, msg) => console.log(`[${new Date().toISOString()}] [${id || '--------'}] ${msg}`);
const run = (cmd, args) => execFileSync(cmd, args, { stdio: ['ignore', 'pipe', 'ignore'] }).toString().trim();

fs.mkdirSync(WORK_ROOT, { recursive: true, mode: 0o711 });
// POSIX named semaphores and shm_open (Lessons 13/14) live in /dev/shm; Cloudflare's VM has none.
// Sticky + world-writable like /tmp, so students can create but not delete each other's objects.
try { fs.mkdirSync('/dev/shm', { recursive: true }); fs.chmodSync('/dev/shm', 0o1777); } catch (e) { log(null, `no /dev/shm: ${e.message}`); }

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

// FIFOs in /tmp (Lesson 08/09) and POSIX semaphores / shared memory in /dev/shm (Lesson 13/14)
// also outlive the session. Unix uids are reused, so a later student would inherit them.
function removeSharedFiles(uid) {
  for (const root of ['/tmp', '/dev/shm', '/var/tmp']) {
    let names;
    try { names = fs.readdirSync(root); } catch { continue; }
    for (const name of names) {
      const p = path.join(root, name);
      try { if (fs.lstatSync(p).uid === uid) fs.rmSync(p, { recursive: true, force: true }); } catch {}
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
  removeSharedFiles(uid);
  fs.rmSync(dir, { recursive: true, force: true });
  try { run('deluser', [user]); } catch {}
}

// Is the program in the foreground of this terminal blocked reading its keyboard?
// /proc/<pid>/stat (readable by root) gives the foreground process group (field 8, tpgid).
// /proc/<pid>/wchan names the kernel function a process sleeps in — but only its own user
// may read it, so the check runs as the session user. A terminal read sleeps in the tty
// layer (n_tty_read / wait_woken); sleep() shows hrtimer_nanosleep, a pipe anon_pipe_read.
function statFields(pid) {
  try {
    const raw = fs.readFileSync(`/proc/${pid}/stat`, 'utf8');
    return raw.slice(raw.lastIndexOf(')') + 2).split(' '); // [0]=state [2]=pgrp [5]=tpgid
  } catch { return null; }
}

function waitingForInput(shellPid, account) {
  const shell = statFields(shellPid);
  if (!shell) return false;
  const fg = Number(shell[5]);
  if (!fg || fg === Number(shell[2])) return false; // the shell itself is in front: no program running
  const members = fs.readdirSync('/proc').filter((d) => /^\d+$/.test(d)).filter((d) => {
    const f = statFields(d);
    return f && Number(f[2]) === fg && f[0] === 'S';
  });
  if (!members.length) return false;
  try {
    const out = execFileSync('cat', members.map((d) => `/proc/${d}/wchan`),
      { uid: account.uid, gid: account.gid, stdio: ['ignore', 'pipe', 'ignore'], timeout: 1000 }).toString();
    return /n_tty_read|wait_woken|tty_read/.test(out);
  } catch { return false; }
}

const server = http.createServer((req, res) => {
  res.setHeader('Access-Control-Allow-Origin', '*');
  if (req.url === '/status') {
    // Same shape as the Worker's /status, for running this container on its own (local development)
    res.writeHead(200, { 'Content-Type': 'application/json' });
    res.end(JSON.stringify({ checkedAt: Date.now(), containers: [{
      name: CONTAINER_NAME, state: 'awake', imageVersion: IMAGE_VERSION, activeSessions: sessions.size, maxSessions: MAX_SESSIONS,
      totalServed: totalSessionsServed, uptimeSeconds: Math.floor((Date.now() - startedAt) / 1000),
    }] }));
    return;
  }
  if (req.url === '/health') {
    res.writeHead(200, { 'Content-Type': 'application/json' });
    res.end(JSON.stringify({
      status: 'ok',
      container: CONTAINER_NAME,
      imageVersion: IMAGE_VERSION,
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
  const panes = new Map(); // pane id -> { pty, waiting }
  let idleTimer = null;
  let hardTimer = null;
  let pollTimer = null;
  let closed = false;

  function cleanup(reason) {
    if (closed) return;
    closed = true;
    clearTimeout(idleTimer);
    clearTimeout(hardTimer);
    clearInterval(pollTimer);
    for (const p of panes.values()) { try { p.pty.kill('SIGKILL'); } catch {} }
    if (account) { try { destroySessionUser(account); } catch (e) { log(sid, `cleanup error: ${e.message}`); } }
    if (sid) { sessions.delete(sid); log(sid, `ended (${reason}); ${sessions.size} active`); }
    try { ws.close(); } catch {}
  }

  function resetIdle() {
    clearTimeout(idleTimer);
    idleTimer = setTimeout(() => {
      send({ type: 'output', pane: 0, data: '\r\n\x1b[33m[Session closed after 3 minutes without activity]\x1b[0m\r\n' });
      cleanup('idle');
    }, IDLE_TIMEOUT_MS);
  }

  // Every pane runs its own bash through the launcher, as the same session user in the same folder
  function openPane(id, mode, cols, rows) {
    // Later panes join the first pane's namespaces (same IPC keys, ports and /tmp as their partner)
    const join = id > 0 && panes.get(0) ? [String(panes.get(0).pty.pid)] : [];
    const p = pty.spawn(LAUNCHER, [String(account.uid), String(account.gid), account.dir, mode, ...join], {
      name: 'xterm-256color',
      cols: Math.min(Math.max(Number(cols) || 80, 20), 400),
      rows: Math.min(Math.max(Number(rows) || 24, 5), 200),
      cwd: account.dir,
      env: { PATH: '/usr/bin:/bin' },
    });
    const pane = { pty: p, waiting: false };
    panes.set(id, pane);
    p.onData((data) => send({ type: 'output', pane: id, data }));
    p.onExit(({ exitCode }) => {
      if (closed) return;
      send({ type: 'exit', pane: id, code: exitCode });
      panes.delete(id);
      if (id === 0) cleanup('shell exited');
    });
  }

  function pollInput() {
    for (const [id, pane] of panes) {
      const waiting = waitingForInput(pane.pty.pid, account);
      if (waiting !== pane.waiting) {
        pane.waiting = waiting;
        send({ type: 'input-wait', pane: id, waiting });
      }
    }
  }

  ws.on('message', (raw) => {
    let msg;
    try { msg = JSON.parse(raw.toString()); } catch { return; }
    resetIdle();
    const pane = panes.get(Number.isInteger(msg.pane) ? msg.pane : 0);

    if (msg.type === 'start') {
      if (account || closed) return;

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
      openPane(0, mode, msg.cols, msg.rows);
      sessions.set(sid, { ws, startedAt: Date.now() });
      totalSessionsServed++;
      log(sid, `started as ${account.user} (uid ${account.uid}) with ${named.length} files, mode=${mode}; ${sessions.size} active`);

      hardTimer = setTimeout(() => {
        send({ type: 'output', pane: 0, data: '\r\n\x1b[33m[Session reached the 20 minute limit — open a new terminal to continue]\x1b[0m\r\n' });
        cleanup('max duration');
      }, MAX_SESSION_MS);
      pollTimer = setInterval(pollInput, INPUT_POLL_MS);
      send({ type: 'ready', sessionId: sid, container: CONTAINER_NAME });

    } else if (msg.type === 'open-pane') {
      // A second (or third) shell in the same sandbox, e.g. server + client side by side
      const id = msg.pane;
      if (!account || closed || !Number.isInteger(id) || id < 1 || id >= MAX_PANES || panes.has(id)) return;
      openPane(id, 'example', msg.cols, msg.rows);
      send({ type: 'pane-ready', pane: id });

    } else if (msg.type === 'input') {
      if (pane && typeof msg.data === 'string' && msg.data.length <= MAX_INPUT_CHARS) pane.pty.write(msg.data);

    } else if (msg.type === 'resize') {
      const cols = Number(msg.cols), rows = Number(msg.rows);
      if (pane && cols >= 20 && cols <= 400 && rows >= 5 && rows <= 200) {
        try { pane.pty.resize(cols, rows); } catch {}
      }

    } else if (msg.type === 'signal') {
      // Writing ^C to the terminal makes the kernel deliver SIGINT to the foreground
      // process group — the running program — not to the (signal-ignoring) shell.
      const key = SIGNAL_KEYS[msg.signal];
      if (pane && key) pane.pty.write(key);
    }
  });

  ws.on('close', () => cleanup('disconnected'));
  ws.on('error', () => cleanup('socket error'));
  resetIdle();
});

server.listen(PORT, () => log(null, `terminal supervisor on :${PORT} (container=${CONTAINER_NAME}, max ${MAX_SESSIONS} sessions)`));
