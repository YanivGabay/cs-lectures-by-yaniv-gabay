import { WebSocketServer } from 'ws';
import http from 'node:http';
import fs from 'node:fs';
import path from 'node:path';
import os from 'node:os';
import { spawn } from 'node:child_process';
import pty from 'node-pty';

const PORT = parseInt(process.env.PORT || '8080');
const MAX_SESSIONS = parseInt(process.env.MAX_SESSIONS || '10');
const SESSION_TIMEOUT_MS = parseInt(process.env.SESSION_TIMEOUT || '300000');
const CONTAINER_IMAGE = process.env.CONTAINER_IMAGE || 'cs-lectures-sandbox';
const ALLOWED_ORIGINS = (process.env.ALLOWED_ORIGINS || 'http://localhost:4321,http://localhost:4322').split(',');

const sessions = new Map();

function log(sessionId, msg) {
  const ts = new Date().toISOString();
  const id = sessionId ? sessionId.slice(0, 8) : '--------';
  console.log(`[${ts}] [${id}] ${msg}`);
}

const server = http.createServer((req, res) => {
  res.setHeader('Access-Control-Allow-Origin', '*');
  res.setHeader('Access-Control-Allow-Methods', 'GET');

  if (req.url === '/health') {
    res.writeHead(200, { 'Content-Type': 'application/json' });
    res.end(JSON.stringify({
      status: 'ok',
      activeSessions: sessions.size,
      maxSessions: MAX_SESSIONS,
      uptime: process.uptime(),
    }));
    return;
  }

  res.writeHead(404);
  res.end('Not found');
});

const wss = new WebSocketServer({
  server,
  verifyClient: ({ origin }) => {
    if (!origin) return true;
    return ALLOWED_ORIGINS.some(o => origin.startsWith(o));
  },
});

wss.on('connection', (ws) => {
  if (sessions.size >= MAX_SESSIONS) {
    ws.send(JSON.stringify({
      type: 'error',
      data: `Server at capacity (${MAX_SESSIONS} sessions). Try again in a few minutes.\r\n`,
    }));
    ws.close();
    return;
  }

  let sessionId = null;
  let ptyProcess = null;
  let tmpDir = null;
  let timeout = null;
  let alive = true;

  function resetTimeout() {
    if (timeout) clearTimeout(timeout);
    timeout = setTimeout(() => {
      if (alive) {
        ws.send(JSON.stringify({
          type: 'output',
          data: '\r\n\x1b[1;33m[Session timed out after 5 minutes of inactivity]\x1b[0m\r\n',
        }));
        cleanup();
        ws.close();
      }
    }, SESSION_TIMEOUT_MS);
  }

  function cleanup() {
    alive = false;
    if (timeout) clearTimeout(timeout);
    if (ptyProcess) {
      try { ptyProcess.kill(); } catch {}
      ptyProcess = null;
    }
    if (sessionId) {
      spawn('docker', ['rm', '-f', `cs-${sessionId.slice(0, 12)}`], { stdio: 'ignore' });
      sessions.delete(sessionId);
      log(sessionId, `Session ended (${sessions.size} active)`);
    }
    if (tmpDir) {
      fs.rm(tmpDir, { recursive: true, force: true }, () => {});
      tmpDir = null;
    }
  }

  ws.on('message', (raw) => {
    let msg;
    try {
      msg = JSON.parse(raw.toString());
    } catch {
      return;
    }

    resetTimeout();

    switch (msg.type) {
      case 'start': {
        if (ptyProcess) {
          ws.send(JSON.stringify({ type: 'error', data: 'Session already running.\r\n' }));
          return;
        }

        sessionId = crypto.randomUUID();
        const containerName = `cs-${sessionId.slice(0, 12)}`;

        tmpDir = fs.mkdtempSync(path.join(os.tmpdir(), 'cs-terminal-'));
        const workDir = path.join(tmpDir, 'work');
        fs.mkdirSync(workDir);

        const files = msg.files || [];
        for (const file of files) {
          const filePath = path.join(workDir, path.basename(file.name));
          fs.writeFileSync(filePath, file.content);
        }

        // Copy .bashrc into workdir so it survives the volume mount
        const bashrcSrc = path.join(path.dirname(new URL(import.meta.url).pathname), 'bashrc');
        if (fs.existsSync(bashrcSrc)) {
          fs.copyFileSync(bashrcSrc, path.join(workDir, '.bashrc'));
        }

        // Make files readable by container user (UID 1000)
        fs.chmodSync(workDir, 0o777);
        for (const f of fs.readdirSync(workDir)) {
          fs.chmodSync(path.join(workDir, f), 0o777);
        }

        const cols = msg.cols || 80;
        const rows = msg.rows || 24;

        const dockerArgs = [
          'run', '-it', '--rm',
          '--name', containerName,
          '--cap-drop', 'ALL',
          '--cap-add', 'SYS_PTRACE',
          '--security-opt', 'no-new-privileges',
          '--memory', '128m',
          '--memory-swap', '128m',
          '--cpus', '0.5',
          '--pids-limit', '64',
          '--network', 'none',
          '--read-only',
          '--tmpfs', '/tmp:rw,exec,nosuid,size=64m',
          '-v', `${workDir}:/home/student:rw`,
          '-e', `TERM=xterm-256color`,
          '-e', `COLUMNS=${cols}`,
          '-e', `LINES=${rows}`,
          CONTAINER_IMAGE,
          '/bin/bash', '--login',
        ];

        ptyProcess = pty.spawn('docker', dockerArgs, {
          name: 'xterm-256color',
          cols,
          rows,
          cwd: process.cwd(),
        });

        sessions.set(sessionId, { ws, ptyProcess, containerName });
        log(sessionId, `Session started with ${files.length} files (${sessions.size} active)`);

        ptyProcess.onData((data) => {
          if (alive && ws.readyState === 1) {
            ws.send(JSON.stringify({ type: 'output', data }));
          }
        });

        ptyProcess.onExit(({ exitCode }) => {
          if (alive && ws.readyState === 1) {
            ws.send(JSON.stringify({
              type: 'exit',
              code: exitCode,
              data: `\r\n\x1b[1;31m[Process exited with code ${exitCode}]\x1b[0m\r\n`,
            }));
          }
          cleanup();
        });

        ws.send(JSON.stringify({ type: 'ready', sessionId }));
        break;
      }

      case 'input': {
        if (ptyProcess && msg.data) {
          ptyProcess.write(msg.data);
        }
        break;
      }

      case 'resize': {
        if (ptyProcess && msg.cols && msg.rows) {
          try {
            ptyProcess.resize(msg.cols, msg.rows);
          } catch {}
        }
        break;
      }

      case 'signal': {
        if (sessionId) {
          const sig = msg.signal || 'SIGINT';
          const containerName = `cs-${sessionId.slice(0, 12)}`;
          spawn('docker', ['kill', '-s', sig, containerName], { stdio: 'ignore' });
          log(sessionId, `Sent ${sig}`);
        }
        break;
      }
    }
  });

  ws.on('close', cleanup);
  ws.on('error', cleanup);
  resetTimeout();
});

server.listen(PORT, () => {
  log(null, `Terminal server listening on port ${PORT}`);
  log(null, `Max sessions: ${MAX_SESSIONS}, timeout: ${SESSION_TIMEOUT_MS / 1000}s`);
  log(null, `Allowed origins: ${ALLOWED_ORIGINS.join(', ')}`);
});

process.on('SIGTERM', () => {
  log(null, 'Shutting down...');
  for (const [id, session] of sessions) {
    try { session.ptyProcess.kill(); } catch {}
    spawn('docker', ['rm', '-f', `cs-${id.slice(0, 12)}`], { stdio: 'ignore' });
  }
  process.exit(0);
});
