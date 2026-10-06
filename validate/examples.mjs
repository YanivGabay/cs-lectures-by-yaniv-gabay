// Example readiness audit: runs every lesson example through the real terminal pipeline,
// exactly like the Run button (one file uploaded, example mode, compile, run), and classifies it.
//   TERMINAL_URL=ws://localhost:8099 node --experimental-strip-types examples.mjs
//   ONLY_LESSON=09 to audit one lesson; RUN_MS=8000 how long a program may run
import WebSocket from 'ws';
import fs from 'node:fs';
import path from 'node:path';
import { fileURLToPath } from 'node:url';
import { stripAnsi, sleep } from './lib.mjs';

const here = path.dirname(fileURLToPath(import.meta.url));
const repo = path.resolve(here, '..');
const { lessons, OS_COURSE, localIncludes } = await import(path.join(repo, 'site/src/data/os-lessons.ts'));

const WS_URL = process.env.TERMINAL_URL || 'ws://localhost:8099';
const ORIGIN = process.env.ORIGIN || 'https://cs-lectures.pages.dev';
const RUN_MS = Number(process.env.RUN_MS || 8000);
const PARALLEL = Number(process.env.PARALLEL || 4);
const ONLY_LESSON = process.env.ONLY_LESSON || '';

const readCode = (lesson, file) => {
  const ex = lesson.examples.find((e) => e.file === file);
  const p = ex?.source ? path.join(repo, ex.source) : path.join(repo, OS_COURSE.basePath, lesson.folder, file);
  try { return fs.readFileSync(p, 'utf8'); } catch { return null; }
};
const runOf = (ex) => ex.runCmd || `./${ex.compileCmd.match(/-o\s+(\S+)/)?.[1] || 'prog'}`;
const base = (f) => f.split('/').pop();

// One sandbox session with any number of panes; resolves events as they arrive
function session(files) {
  const ws = new WebSocket(WS_URL, { headers: { Origin: ORIGIN } });
  const panes = new Map();
  const pane = (i) => {
    if (!panes.has(i)) panes.set(i, { out: '', scanned: 0, statuses: [], waits: 0, waiting: false, lastOutput: Date.now() });
    return panes.get(i);
  };
  const s = {
    ws, pane,
    send: (m) => ws.readyState === 1 && ws.send(JSON.stringify(m)),
    type: (i, text) => s.send({ type: 'input', pane: i, data: text }),
    ready: new Promise((resolve, reject) => {
      ws.on('open', () => ws.send(JSON.stringify({ type: 'start', mode: 'example', cols: 120, rows: 40, files })));
      ws.on('error', reject);
      ws.on('message', (raw) => {
        const m = JSON.parse(raw);
        const p = pane(m.pane ?? 0);
        if (m.type === 'ready') resolve();
        else if (m.type === 'error') reject(new Error(m.data.trim()));
        else if (m.type === 'output') {
          p.out += m.data;
          p.lastOutput = Date.now();
          // scan the whole stream, not single messages: an escape sequence can be split in two
          const re = /\x1b\]777;cs;prompt;(\d+)\x07/g;
          re.lastIndex = p.scanned;
          for (let x; (x = re.exec(p.out)); ) { p.statuses.push(Number(x[1])); p.scanned = re.lastIndex; }
        } else if (m.type === 'input-wait') { p.waiting = m.waiting; if (m.waiting) p.waits++; }
      });
    }),
    close: () => { try { ws.close(); } catch {} },
  };
  return s;
}

const until = async (fn, ms) => { const end = Date.now() + ms; while (Date.now() < end) { if (fn()) return true; await sleep(100); } return false; };

// Compile in pane i; returns null on success or the compiler's complaint
async function compile(s, i, ex) {
  const p = s.pane(i);
  await until(() => p.statuses.length >= 1, 15000);
  const n = p.statuses.length;
  s.type(i, `${ex.compileCmd}\n`);
  if (!(await until(() => p.statuses.length > n, 60000))) return 'compile timed out';
  if (p.statuses.at(-1) === 0) return null;
  return stripAnsi(p.out).split('\n').filter((l) => /error|undefined reference|No such file/.test(l)).slice(0, 3).join(' / ') || 'compile failed';
}

// Run in pane i; feeds sampleInput when the program asks
async function run(s, i, ex, ms = RUN_MS) {
  const p = s.pane(i);
  const n = p.statuses.length;
  const outStart = p.out.length;
  s.type(i, `${runOf(ex)}\n`);
  let fed = false;
  const started = Date.now();
  const end = Date.now() + ms;
  while (Date.now() < end && p.statuses.length === n) {
    const quiet = Date.now() - p.lastOutput > 1500 && Date.now() - started > 1500;
    if ((p.waiting || quiet) && ex.sampleInput && !fed) { fed = true; s.type(i, ex.sampleInput.endsWith('\n') ? ex.sampleInput : `${ex.sampleInput}\n`); }
    await sleep(100);
  }
  const finished = p.statuses.length > n;
  const r = {
    finished, status: finished ? p.statuses.at(-1) : null, askedInput: p.waits > 0, fed,
    silentFor: Date.now() - p.lastOutput, waitingNow: p.waiting,
  };
  if (!finished) {
    s.send({ type: 'signal', pane: i, signal: ex.stopSignal || 'SIGINT' });
    if (!(await until(() => p.statuses.length > n, 3000))) {
      r.ignoresStop = true; // e.g. a demo that ignores Ctrl+C — needs stopSignal: 'SIGQUIT'
      s.send({ type: 'signal', pane: i, signal: 'SIGQUIT' });
      await until(() => p.statuses.length > n, 3000);
    }
  }
  r.output = stripAnsi(p.out.slice(outStart)).replace(/\x1b\][^\x07]*\x07/g, '').replace(/\x1b\[\?2004[hl]/g, '').trim();
  return r;
}

// What the "Expected Output" panel shows: the run as a student sees it, without the echoed
// command and the prompt that follows; long demos say where they were stopped
function captureText(ex, category, r) {
  if (!['runs-and-exits', 'runs-with-input', 'long-running', 'crashes'].includes(category)) return undefined;
  const lines = r.output.split('\n');
  if (lines[0].includes(runOf(ex).split(' ')[0])) lines.shift();
  while (lines.length && /^\s*(student:.*\$\s*)?$/.test(lines.at(-1))) lines.pop();
  let text = lines.join('\n').replace(/\s+$/, '');
  if (category === 'long-running') text += `\n\n[runs until stopped — the site stops it after ${Math.round(ex.stopAfterMs / 1000)}s]`;
  return text;
}

function classify(ex, c, r, pr) {
  if (c) {
    if (/windows\.h|process\.h|conio\.h/i.test(c)) return ['windows-only', c];
    if (/undefined reference to .?main/.test(c)) return ['reference-only', c];
    return ['compile-error', c];
  }
  const res = pr ? [r, pr] : [r];
  // 127 = "command not found": typically sample input typed into bash after the program ended
  if (res.some((x) => x.status === 127 || /command not found/.test(x.output || ''))) return ['command-not-found', 'something was typed into the shell instead of the program'];
  const crashed = res.find((x) => x.finished && x.status > 128 && x.status !== 130);
  if (crashed) return ['crashes', `status ${crashed.status}`];
  if (res.every((x) => x.finished)) {
    const st = res.map((x) => x.status);
    if (r.askedInput) return ['runs-with-input', `status ${st.join('/')}${r.fed ? ', fed sampleInput' : ''}`];
    return ['runs-and-exits', `status ${st.join('/')}`];
  }
  if (res.some((x) => x.askedInput && !x.fed)) return ['needs-input', 'waiting for keyboard input'];
  if (pr) return ['pair-hangs', 'pair did not finish'];
  if (r.silentFor < 1500) return ['long-running', 'still printing when stopped'];
  return ['hangs', `no output for ${(r.silentFor / 1000).toFixed(1)}s while blocked`];
}

async function audit(lesson, ex) {
  const code = readCode(lesson, ex.file);
  const item = { lesson: lesson.number, file: ex.file, kind: ex.kind || '', intentional: ex.intentional || '' };
  if (code === null) return { ...item, category: 'missing-file', detail: 'source not found' };
  if (ex.kind === 'reference' || ex.kind === 'windows') return { ...item, category: `${ex.kind === 'windows' ? 'windows-only' : 'reference-only'}`, detail: 'marked in data (not run)' };
  const starter = lesson.examples.find((e) => e.pairWith === ex.file);
  if (starter) return { ...item, category: 'pair-partner', detail: `runs with ${starter.file} (Run pair)` };
  const partner = ex.pairWith && lesson.examples.find((e) => e.file === ex.pairWith);
  // Same files the page uploads: the example, its partner, and headers they include
  const files = [{ name: ex.file, content: code }];
  if (partner) files.push({ name: partner.file, content: readCode(lesson, partner.file) });
  for (const extra of ex.extraFiles || []) files.push({ name: extra, content: readCode(lesson, extra) });
  for (const f of [...files]) {
    for (const h of localIncludes(f.name, f.content || '')) {
      const c = readCode(lesson, h);
      if (c !== null && !files.some((x) => x.name === h)) files.push({ name: h, content: c });
    }
  }
  const s = session(files);
  try {
    await s.ready;
    if (ex.companion) {
      s.send({ type: 'open-pane', pane: 1, cols: 120, rows: 40 });
      const c = await compile(s, 0, ex);
      if (c) { const [category, detail] = classify(ex, c); return { ...item, category, detail }; }
      await until(() => s.pane(1).statuses.length >= 1, 15000);
      const first = run(s, 0, ex, ex.stopAfterMs ? ex.stopAfterMs + 500 : RUN_MS);
      await sleep(ex.companion.startDelayMs ?? 800);
      const second = run(s, 1, { runCmd: ex.companion.cmd, compileCmd: '' });
      const [r, cr] = await Promise.all([first, second]);
      let [category, detail] = classify(ex, null, r);
      if (ex.stopAfterMs && !r.finished && !r.ignoresStop) [category, detail] = ['long-running', `stopped after ${ex.stopAfterMs}ms as configured`];
      return { ...item, category, detail: `${detail}; companion status ${cr.status}`, output: `${r.output.slice(-300)}\n--- companion ---\n${cr.output.slice(-200)}` };
    }
    if (partner) {
      s.send({ type: 'open-pane', pane: 1, cols: 120, rows: 40 });
      const [c1, c2] = await Promise.all([compile(s, 0, ex), compile(s, 1, partner)]);
      if (c1 || c2) return { ...item, category: 'compile-error', detail: c1 || c2 };
      const first = run(s, 0, ex, ex.stopAfterMs ? ex.stopAfterMs + 500 : RUN_MS);
      await sleep(ex.startDelayMs ?? 800);
      const second = run(s, 1, partner, partner.stopAfterMs ? partner.stopAfterMs + 500 : RUN_MS);
      const [r, pr] = await Promise.all([first, second]);
      let [category, detail] = classify(ex, null, r, pr);
      // A server that keeps listening is fine once its client finished and it stopped on schedule
      const done = (x, cfg) => x.finished || (cfg.stopAfterMs && !x.ignoresStop);
      if (category === 'pair-hangs' && done(r, ex) && done(pr, partner)) [category, detail] = ['pair-runs', `statuses ${r.status ?? 'stopped'}/${pr.status ?? 'stopped'} (stopped on schedule)`];
      if (category === 'runs-and-exits' || category === 'runs-with-input') category = 'pair-runs';
      return { ...item, category, detail, output: `${r.output.slice(-300)}\n--- ${base(partner.file)} ---\n${pr.output.slice(-300)}` };
    }
    const c = await compile(s, 0, ex);
    if (c) { const [category, detail] = classify(ex, c); return { ...item, category, detail }; }
    const r = await run(s, 0, ex, ex.stopAfterMs ? ex.stopAfterMs + 500 : RUN_MS);
    let [category, detail] = classify(ex, null, r);
    if (ex.stopAfterMs && !r.finished && !r.ignoresStop) [category, detail] = ['long-running', `stopped after ${ex.stopAfterMs}ms as configured`];
    if (r.ignoresStop) [category, detail] = ['ignores-stop', `${ex.stopSignal || 'SIGINT'} did not stop it (needed SIGQUIT)`];
    return { ...item, category, detail, output: r.output.slice(-400), capture: captureText(ex, category, r) };
  } catch (e) {
    return { ...item, category: 'session-error', detail: e.message };
  } finally {
    s.close();
  }
}

const jobs = [];
for (const lesson of lessons) {
  if (ONLY_LESSON && lesson.number !== ONLY_LESSON) continue;
  for (const ex of lesson.examples) jobs.push([lesson, ex]);
}
const results = [];
let next = 0;
await Promise.all(Array.from({ length: PARALLEL }, async () => {
  while (next < jobs.length) {
    const [lesson, ex] = jobs[next++];
    const r = await audit(lesson, ex);
    results.push(r);
    console.log(`${r.category.padEnd(16)} L${r.lesson} ${r.file} — ${r.detail}`);
  }
}));

// Anything in these buckets must be explained by the data (kind / intentional) — see T13
const PROBLEM = ['command-not-found', 'ignores-stop', 'crashes', 'hangs', 'compile-error', 'needs-input', 'pair-hangs', 'missing-file', 'session-error'];
// An intentional note may excuse a crash (the point of the example) — never a hang or a compile error
const unexplained = results.filter((r) => PROBLEM.includes(r.category) && !(r.intentional && r.category === 'crashes'));
const counts = results.reduce((m, r) => ({ ...m, [r.category]: (m[r.category] || 0) + 1 }), {});

const order = (r) => [Number(r.lesson), r.file];
results.sort((a, b) => (order(a)[0] - order(b)[0]) || a.file.localeCompare(b.file));
fs.mkdirSync(path.join(here, 'reports'), { recursive: true });
const md = [`# Example readiness — ${new Date().toISOString().slice(0, 16).replace('T', ' ')} UTC`, '',
  `Terminal: \`${WS_URL}\` · run limit ${RUN_MS / 1000}s · ${results.length} examples`, '',
  '| Category | Count |', '|---|---|', ...Object.entries(counts).sort().map(([k, v]) => `| ${k} | ${v} |`), '',
  `Unexplained problems: **${unexplained.length}**`, ''];
for (const lesson of lessons) {
  const rows = results.filter((r) => r.lesson === lesson.number);
  if (!rows.length) continue;
  md.push(`## ${lesson.number} — ${lesson.title}`, '', '| File | Category | Detail | Data |', '|---|---|---|---|');
  for (const r of rows) md.push(`| \`${r.file}\` | ${r.category} | ${String(r.detail).replace(/\|/g, '\\|').replace(/\n/g, ' ')} | ${[r.kind, r.intentional && 'intentional'].filter(Boolean).join(', ')} |`);
  md.push('');
}
const tag = ONLY_LESSON ? `-L${ONLY_LESSON}` : '';
fs.writeFileSync(path.join(here, `reports/examples${tag}.md`), md.join('\n'));
// CAPTURE=1: refresh the site's "Expected Output" panels from these sandbox runs
if (process.env.CAPTURE && !ONLY_LESSON) {
  const captured = {};
  for (const r of results) {
    const lesson = lessons.find((l) => l.number === r.lesson);
    if (r.capture) captured[`${lesson.folder}/${r.file}`] = r.capture;
  }
  fs.writeFileSync(path.join(repo, 'site/src/data/captured-outputs.json'), JSON.stringify(captured, null, 2) + '\n');
  console.log(`captured ${Object.keys(captured).length} expected outputs`);
}
fs.writeFileSync(path.join(here, `reports/examples${tag}.json`), JSON.stringify({ counts, unexplained: unexplained.length, results }, null, 2));
console.log(`\n${JSON.stringify(counts)}\nunexplained problems: ${unexplained.length}`);
process.exit(process.env.STRICT && unexplained.length ? 1 : 0);
