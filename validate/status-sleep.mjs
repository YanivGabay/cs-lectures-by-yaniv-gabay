// Does polling /status (as the status page does every 10s) keep a container awake?
// Wakes one student container, polls like the page, and waits for it to fall asleep (sleepAfter 5m).
//   TERMINAL_URL=wss://cs-lectures-terminal.yaniv242.workers.dev node status-sleep.mjs
import WebSocket from 'ws';
import { sleep } from './lib.mjs';
const WS = process.env.TERMINAL_URL || 'wss://cs-lectures-terminal.yaniv242.workers.dev';
const HTTP = WS.replace(/^ws/, 'http');
const status = async () => (await (await fetch(`${HTTP}/status`)).json()).containers;

const ws = new WebSocket(WS, { headers: { Origin: 'https://cs-lectures.pages.dev' } });
const name = await new Promise((resolve) => {
  ws.on('open', () => ws.send(JSON.stringify({ type: 'start', mode: 'example', files: [] })));
  ws.on('message', (d) => { const m = JSON.parse(d); if (m.type === 'ready') resolve(m.container); });
});
await sleep(2000);
ws.close();
const t0 = Date.now();
console.log(`woke ${name}; polling /status every 10s`);
for (;;) {
  const c = (await status()).find((x) => x.name === name);
  const min = ((Date.now() - t0) / 60000).toFixed(1);
  if (c.state === 'asleep') { console.log(`PASS ${name} fell asleep ${min} min after its last session while /status was polled`); process.exit(0); }
  if (Date.now() - t0 > 12 * 60000) { console.log(`FAIL ${name} still ${c.state} after ${min} min of polling — /status keeps it awake`); process.exit(1); }
  await sleep(10000);
}
