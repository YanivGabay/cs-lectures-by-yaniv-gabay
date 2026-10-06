// Adversarial probes against the terminal backend.
//   Local container:  TERMINAL_URL=ws://localhost:8099 DESTRUCTIVE=1 SKIP_NETWORK=1 node security.mjs
//   Production:       TERMINAL_URL=wss://cs-lectures-terminal.yaniv242.workers.dev node security.mjs
import WebSocket from 'ws';
import fs from 'node:fs';
import os from 'node:os';
import path from 'node:path';
import { check, assert, finish, stripAnsi, sleep, waitFor } from './lib.mjs';

const WS_URL = process.env.TERMINAL_URL || 'ws://localhost:8099';
const HTTP_URL = WS_URL.replace(/^ws/, 'http');
const DESTRUCTIVE = process.env.DESTRUCTIVE === '1';
const BEHIND_WORKER = WS_URL.startsWith('wss://');
const ORIGIN = process.env.ORIGIN || 'https://cs-lectures.pages.dev';

let seq = 0;

function presenterKey() {
  if (process.env.PRESENTER_KEY) return process.env.PRESENTER_KEY;
  try { return fs.readFileSync(path.join(os.homedir(), '.cs-lectures-presenter-key'), 'utf8').trim(); } catch { return ''; }
}

function connect(origin = ORIGIN, query = '') {
  const ws = new WebSocket(WS_URL + query, { headers: { Origin: origin } });
  return new Promise((resolve, reject) => {
    ws.once('open', () => resolve(ws));
    ws.once('unexpected-response', (_req, res) => reject(new Error(`HTTP ${res.statusCode}`)));
    ws.once('error', reject);
  });
}

async function openSession(files = [{ name: 'hello.c', content: 'int main(){return 0;}\n' }], mode = 'lesson', query = '') {
  const ws = await connect(ORIGIN, query);
  const s = { ws, out: '', error: '', closed: false, container: null };
  ws.on('message', (d) => {
    try {
      const m = JSON.parse(d);
      if (m.type === 'output') s.out += m.data;
      if (m.type === 'error') s.error += m.data;
      if (m.type === 'ready') s.container = m.container;
    } catch {}
  });
  ws.on('close', () => { s.closed = true; });
  ws.send(JSON.stringify({ type: 'start', files, cols: 120, rows: 30, mode }));
  await waitFor(() => /[$#] $/m.test(stripAnsi(s.out)) || s.closed || s.error, 30000, 'shell prompt');
  if (s.error) throw new Error(`server refused session: ${s.error.trim()}`);
  s.send = (data) => ws.send(JSON.stringify({ type: 'input', data }));
  s.run = async (cmd, ms = 8000) => {
    const id = ++seq;
    const start = s.out.length;
    const sep = cmd.trim().endsWith('&') ? ' ' : '; '; // "cmd &; echo" is a bash syntax error
    s.send(`${cmd}${sep}echo __M${id}_$?__\r`);
    const re = new RegExp(`__M${id}_(\\d+)__`);
    await waitFor(() => re.test(stripAnsi(s.out.slice(start))) || s.closed, ms, `result of: ${cmd}`);
    const text = stripAnsi(s.out.slice(start));
    const m = text.match(re);
    if (!m) throw new Error(`session died while running: ${cmd}`);
    return text.slice(text.indexOf('\n') + 1, m.index).trim();
  };
  s.close = () => { try { ws.close(); } catch {} };
  return s;
}

async function healthy() {
  try { return (await fetch(`${HTTP_URL}/health`)).ok; } catch { return false; }
}

await check('S01', 'shell does not run as root', async () => {
  const a = await openSession();
  const uid = await a.run('id -u');
  a.close();
  assert(uid !== '0', 'uid is 0 (root)');
  return `uid=${uid}`;
});

await check('S02', 'resource limits are set (processes, CPU, memory, file size, open files)', async () => {
  const a = await openSession();
  const raw = await a.run('echo "$(ulimit -u) $(ulimit -t) $(ulimit -v) $(ulimit -f) $(ulimit -n)"');
  a.close();
  const [nproc, cpu, mem, fsize, nofile] = raw.split(/\s+/);
  const bad = Object.entries({ nproc, cpu, mem, fsize, nofile }).filter(([, v]) => v === 'unlimited').map(([k]) => k);
  assert(!bad.length, `unlimited: ${bad.join(', ')}`);
  assert(Number(nproc) <= 256, `process limit too high (${nproc})`);
  return raw;
});

await check('S03', "a student cannot read or write another student's files", async () => {
  const a = await openSession();
  const b = await openSession();
  const dir = await a.run('echo SECRET_9f3 > secret.txt && pwd');
  const read = await b.run(`cat ${dir}/secret.txt 2>&1`);
  await b.run(`echo pwned > ${dir}/evil.txt 2>/dev/null`);
  const listing = await a.run('ls');
  a.close(); b.close();
  assert(!read.includes('SECRET_9f3'), 'other session read the secret file');
  assert(!listing.includes('evil.txt'), 'other session wrote into my directory');
});

await check('S04', 'server code is not writable from the shell', async () => {
  const a = await openSession();
  const r = await a.run('(echo x >> /app/server.js) 2>/dev/null && echo WROTE || echo DENIED');
  a.close();
  assert(r.includes('DENIED'), '/app/server.js is writable');
});

await check('S05', 'background processes are killed when a session ends', async () => {
  const d = await openSession();
  await d.run('nohup sleep 777 >/dev/null 2>&1 &');
  d.close();
  await sleep(3000);
  const a = await openSession();
  const n = await a.run("pgrep -f 'slee[p] 777' | wc -l");
  a.close();
  assert(n.trim() === '0', `${n.trim()} leftover process(es) still running`);
});

await check('S06', 'unsafe file names are refused; files in different folders are kept apart', async () => {
  for (const name of ['../../../tmp/pwn.txt', '/tmp/pwn_abs.txt', '.bashrc']) {
    const err = await openSession([{ name, content: 'x' }]).then((s) => { s.close(); return null; }, (e) => e.message);
    assert(err && /Invalid file name/.test(err), `"${name}" was accepted (${err || 'session opened'})`);
  }
  const s = await openSession([{ name: 'a/one.c', content: 'ONE' }, { name: 'b/one.c', content: 'TWO' }]);
  const r = await s.run('cat a/one.c; echo; cat b/one.c');
  s.close();
  assert(/ONE\s+TWO/.test(r), `same-named files in different folders collided: ${r}`);
});

await check('S07', 'oversized uploads are rejected', async () => {
  const ws = await connect();
  let gotError = false; let ready = false;
  ws.on('message', (d) => { const m = JSON.parse(d); if (m.type === 'error') gotError = true; if (m.type === 'ready') ready = true; });
  const files = Array.from({ length: 300 }, (_, i) => ({ name: `f${i}.c`, content: 'x'.repeat(10_000) }));
  ws.send(JSON.stringify({ type: 'start', files, cols: 80, rows: 24 }));
  await sleep(4000);
  ws.close();
  assert(gotError || !ready, 'server accepted 300 files / 3 MB');
});

await check('S08', 'Ctrl+C (signal message) interrupts the running program', async () => {
  const a = await openSession();
  a.send('sleep 100\r');
  await sleep(1200);
  a.ws.send(JSON.stringify({ type: 'signal', signal: 'SIGINT' }));
  const r = await a.run('echo INT_OK', 5000).catch((e) => e.message);
  a.close();
  assert(r.includes('INT_OK'), 'shell still blocked by sleep after SIGINT');
});

await check('S14', 'orphaned processes are reaped (no zombies pile up)', async () => {
  const a = await openSession();
  await a.run("sh -c '(sleep 0.3 &)'; sleep 2");
  const z = await a.run("ps -e -o stat= | grep -c '^Z' || true");
  a.close();
  assert(z.trim() === '0', `${z.trim()} zombie process(es) left`);
});

await check('S15', 'System V IPC objects are removed when a session ends', async () => {
  const a = await openSession();
  const uid = await a.run('id -u');
  await a.run(`printf '#include <sys/msg.h>\\n#include <sys/shm.h>\\nint main(void){msgget(IPC_PRIVATE,0600|IPC_CREAT);shmget(IPC_PRIVATE,4096,0600|IPC_CREAT);return 0;}\\n' > ipc.c && gcc -o ipc ipc.c && ./ipc`, 20000);
  const before = await a.run(`cat /proc/sysvipc/msg /proc/sysvipc/shm | awk -v u=${uid} '$0 !~ /key/ && $8==u' | wc -l`);
  a.close();
  await sleep(3000);
  const b = await openSession();
  const after = await b.run(`cat /proc/sysvipc/msg /proc/sysvipc/shm | awk -v u=${uid} '$0 !~ /key/ && $8==u' | wc -l`);
  b.close();
  assert(Number(before) >= 1, `test setup failed — no IPC objects created (${before})`);
  assert(after.trim() === '0', `${after.trim()} IPC object(s) still owned by uid ${uid} after disconnect`);
});

if (DESTRUCTIVE) {
  await check('S09', 'kill -9 -1 from one student does not kill other sessions', async () => {
    const a = await openSession();
    const b = await openSession();
    b.send('kill -9 -1\r');
    await sleep(2000);
    const r = await a.run('echo STILL_ALIVE', 8000).catch((e) => e.message);
    a.close(); b.close();
    assert(r.includes('STILL_ALIVE'), `other session died: ${r}`);
    assert(await healthy(), 'server /health is down');
  });

  await check('S10', 'a fork bomb is contained to the attacker', async () => {
    const a = await openSession();
    const c = await openSession();
    c.send(':(){ :|:& };:\r');
    await sleep(8000);
    // /bin/echo needs fork+exec — a builtin echo would "survive" even with the process table full
    const r = await a.run('/bin/echo SURV$((1+1))IVED', 15000).catch((e) => e.message);
    c.close();
    await sleep(4000);
    const ok = await healthy();
    const left = await a.run("ps -e -o pid= | wc -l", 10000).catch((e) => e.message);
    a.close();
    assert(r.includes('SURV2IVED'), `other session could not start a program during the fork bomb: ${r.slice(0, 120)}`);
    assert(ok, 'server /health is down after fork bomb');
    assert(Number(left) < 100, `fork bomb processes still alive after the attacker left (${left.trim()} processes)`);
  });
}

if (!process.env.SKIP_NETWORK) {
  await check('S11', 'no outbound network from the sandbox', async () => {
    const a = await openSession();
    const r = await a.run('wget -q -T 3 -O /dev/null http://1.1.1.1 && echo NET_OPEN || echo NET_BLOCKED', 12000);
    a.close();
    assert(r.includes('NET_BLOCKED'), 'outbound HTTP succeeded');
  });
}

if (BEHIND_WORKER) {
  await check('S16', '11 students at once all get a working shell', async () => {
    const opened = await Promise.allSettled(Array.from({ length: 11 }, () => openSession()));
    const ok = opened.filter((r) => r.status === 'fulfilled').map((r) => r.value);
    const failed = opened.filter((r) => r.status === 'rejected').map((r) => r.reason.message);
    const spread = ok.reduce((m, s) => ({ ...m, [s.container]: (m[s.container] || 0) + 1 }), {});
    ok.forEach((s) => s.close());
    assert(!failed.length, `${failed.length}/11 failed: ${failed[0]}`);
    return JSON.stringify(spread);
  });

  await check('S17', 'the presenter key reaches the reserved container; students never do', async () => {
    const key = presenterKey();
    assert(key, 'no presenter key (set PRESENTER_KEY or ~/.cs-lectures-presenter-key)');
    const p = await openSession(undefined, 'lesson', `?presenter=${encodeURIComponent(key)}`);
    const s = await openSession(undefined, 'lesson', '?presenter=wrong-key');
    p.close(); s.close();
    assert(p.container === 'presenter', `presenter session landed on ${p.container}`);
    assert(String(s.container).startsWith('pool-'), `wrong key landed on ${s.container}`);
  });

  await check('S12', 'connections from other websites are refused', async () => {
    const err = await connect('https://evil.example.com').then((ws) => { ws.close(); return null; }, (e) => e.message);
    assert(err && /HTTP 403/.test(err), `foreign origin was accepted (${err || 'opened'})`);
  });

  await check('S13', 'connection flooding is rate limited', async () => {
    let limited = 0;
    // limit is 100/min per IP (a classroom shares one IP) — 200 attempts must trip it
    for (let i = 0; i < 200 && !limited; i++) {
      await connect().then((ws) => ws.close(), (e) => { if (/HTTP 429/.test(e.message)) limited++; });
    }
    assert(limited, '200 rapid connections were all accepted');
  });
}

finish();
process.exit(process.exitCode); // leftover sockets from killed sessions would keep Node alive
