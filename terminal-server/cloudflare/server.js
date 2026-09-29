import { WebSocketServer } from 'ws';
import http from 'node:http';
import fs from 'node:fs';
import path from 'node:path';
import pty from 'node-pty';

const PORT = parseInt(process.env.PORT || '8080');
const MAX_SESSIONS = parseInt(process.env.MAX_SESSIONS || '5');
const SESSION_TIMEOUT_MS = parseInt(process.env.SESSION_TIMEOUT || '300000');

const sessions = new Map();

function log(id, msg) {
  console.log(`[${new Date().toISOString()}] [${id || '----'}] ${msg}`);
}

const server = http.createServer((req, res) => {
  if (req.url === '/health') {
    res.writeHead(200, { 'Content-Type': 'application/json' });
    res.end(JSON.stringify({ status: 'ok', sessions: sessions.size, max: MAX_SESSIONS }));
    return;
  }
  res.writeHead(404);
  res.end();
});

const wss = new WebSocketServer({ server });

wss.on('connection', (ws) => {
  if (sessions.size >= MAX_SESSIONS) {
    ws.send(JSON.stringify({ type: 'error', data: `Server at capacity (${MAX_SESSIONS} sessions).\r\n` }));
    ws.close();
    return;
  }

  let sessionId = null;
  let ptyProcess = null;
  let workDir = null;
  let timeout = null;
  let alive = true;

  function resetTimeout() {
    if (timeout) clearTimeout(timeout);
    timeout = setTimeout(() => {
      if (alive) {
        ws.send(JSON.stringify({ type: 'output', data: '\r\n\x1b[33m[Session timed out]\x1b[0m\r\n' }));
        cleanup();
        ws.close();
      }
    }, SESSION_TIMEOUT_MS);
  }

  function cleanup() {
    alive = false;
    if (timeout) clearTimeout(timeout);
    if (ptyProcess) { try { ptyProcess.kill(); } catch {} }
    if (sessionId) {
      sessions.delete(sessionId);
      log(sessionId, `Session ended (${sessions.size} active)`);
    }
    if (workDir) { fs.rm(workDir, { recursive: true, force: true }, () => {}); }
  }

  ws.on('message', (raw) => {
    let msg;
    try { msg = JSON.parse(raw.toString()); } catch { return; }
    resetTimeout();

    if (msg.type === 'start') {
      if (ptyProcess) return;

      sessionId = crypto.randomUUID().slice(0, 8);

      // Create temp working directory for this session
      workDir = `/home/student/session-${sessionId}`;
      fs.mkdirSync(workDir, { recursive: true });

      // Write files
      const files = msg.files || [];
      for (const file of files) {
        fs.writeFileSync(path.join(workDir, path.basename(file.name)), file.content);
      }

      const cols = msg.cols || 80;
      const rows = msg.rows || 24;

      // Spawn bash directly as student user — no Docker needed
      ptyProcess = pty.spawn('/bin/bash', ['--login'], {
        name: 'xterm-256color',
        cols,
        rows,
        cwd: workDir,
        env: {
          ...process.env,
          TERM: 'xterm-256color',
          HOME: '/home/student',
          USER: 'student',
          COLUMNS: String(cols),
          LINES: String(rows),
        },
      });

      sessions.set(sessionId, { ws, ptyProcess });
      log(sessionId, `Session started with ${files.length} files (${sessions.size} active)`);

      ptyProcess.onData((data) => {
        if (alive && ws.readyState === 1) {
          ws.send(JSON.stringify({ type: 'output', data }));
        }
      });

      ptyProcess.onExit(({ exitCode }) => {
        if (alive && ws.readyState === 1) {
          ws.send(JSON.stringify({ type: 'exit', code: exitCode }));
        }
        cleanup();
      });

      ws.send(JSON.stringify({ type: 'ready', sessionId }));

    } else if (msg.type === 'input') {
      if (ptyProcess && msg.data) ptyProcess.write(msg.data);

    } else if (msg.type === 'resize') {
      if (ptyProcess && msg.cols && msg.rows) {
        try { ptyProcess.resize(msg.cols, msg.rows); } catch {}
      }
    }
  });

  ws.on('close', cleanup);
  ws.on('error', cleanup);
  resetTimeout();
});

server.listen(PORT, () => {
  log(null, `Terminal server listening on port ${PORT}`);
});
