// Live terminal shared by the Run buttons, "Run pair" and the full lesson terminal.
//
// The sandbox shell reports what happens through invisible escape sequences (OSC 777, see
// terminal-server/cloudflare/cs-bashrc): "exec" when a typed command starts and "prompt;<status>"
// when it finished. The server adds "input-wait" while the running program blocks reading the
// keyboard. Everything below (auto compile-then-run, exit chips, input hints) is driven by those
// events rather than by guessing from the text on screen.
import { Terminal } from '@xterm/xterm';
import { FitAddon } from '@xterm/addon-fit';

export interface PanePlan {
  label?: string;
  compile?: string;       // e.g. "gcc -o prog file.c"; omitted = no auto compile
  run?: string;           // e.g. "./prog"; derived from compile's -o when omitted
  startAfterMs?: number;  // pair: run this pane's program this long after the first pane's starts
  sampleInput?: string;   // typed by "Insert sample input"
  stopAfterMs?: number;   // long-running demo: stop it after this long...
  stopSignal?: 'SIGINT' | 'SIGQUIT'; // ...with Ctrl+C (default) or Ctrl+\ for programs that ignore Ctrl+C
  interruptAfterMs?: number; // send one Ctrl+C after this long (challenges: "survive Ctrl+C")
}

export interface TerminalOptions {
  files: { name: string; content: string }[];
  mode: 'example' | 'lesson';
  panes: PanePlan[];
  title?: string;
  height?: number;
  onReady?: () => void;
}

// What a signal means, in the words used in the lectures
const SIGNALS: Record<number, [string, string]> = {
  1: ['SIGHUP', 'terminal hung up'],
  2: ['SIGINT', 'interrupted (Ctrl+C)'],
  3: ['SIGQUIT', 'quit (Ctrl+\\)'],
  4: ['SIGILL', 'illegal instruction'],
  6: ['SIGABRT', 'aborted — abort() or a failed assert'],
  8: ['SIGFPE', 'arithmetic error, e.g. division by zero'],
  9: ['SIGKILL', 'killed — cannot be caught'],
  10: ['SIGUSR1', 'user signal 1 with no handler'],
  11: ['SIGSEGV', 'invalid memory access'],
  12: ['SIGUSR2', 'user signal 2 with no handler'],
  13: ['SIGPIPE', 'wrote to a pipe or socket nobody reads'],
  14: ['SIGALRM', 'alarm() timer went off with no handler'],
  15: ['SIGTERM', 'asked to terminate'],
  24: ['SIGXCPU', 'CPU time limit reached'],
  25: ['SIGXFSZ', 'file size limit reached'],
};

export function describeStatus(code: number): { text: string; tone: 'ok' | 'warn' | 'bad' } {
  if (code === 0) return { text: 'Exited 0', tone: 'ok' };
  // bash reports a process killed by signal N as 128+N
  if (code > 128 && code < 160) {
    const n = code - 128;
    const [name, why] = SIGNALS[n] || [`signal ${n}`, ''];
    return { text: `Killed by ${name} (${n})${why ? ` — ${why}` : ''}`, tone: n === 2 ? 'warn' : 'bad' };
  }
  if (code === 127) return { text: 'Exited 127 — command not found', tone: 'bad' };
  return { text: `Exited ${code}`, tone: 'warn' };
}

const TONE: Record<string, string> = {
  ok: 'bg-emerald-900/60 text-emerald-200 border-emerald-700',
  warn: 'bg-amber-900/60 text-amber-200 border-amber-700',
  bad: 'bg-red-900/70 text-red-200 border-red-700',
  info: 'bg-sky-900/60 text-sky-200 border-sky-700',
};

function getTerminalUrl(): string {
  const meta = document.querySelector('meta[name="terminal-url"]') as HTMLMetaElement | null;
  const base = meta?.content || localStorage.getItem('terminal-url') || '';
  let key: string | null = null;
  try { key = localStorage.getItem('presenterKey'); } catch {}
  // The presenter's browser goes to the reserved sandbox; everyone else to the student pool
  return base && key ? `${base}${base.includes('?') ? '&' : '?'}presenter=${encodeURIComponent(key)}` : base;
}

export function fontSize(): number {
  let n = 13;
  try { n = Number(localStorage.getItem('termFontSize')) || 13; } catch {}
  return Math.min(Math.max(n, 10), 32);
}

import { OSC, takeComplete } from './osc';
const runOf = (p: PanePlan) => p.run || (p.compile ? `./${p.compile.match(/-o\s+(\S+)/)?.[1] || 'prog'}` : '');

export function openTerminal(container: HTMLElement, opts: TerminalOptions) {
  const multi = opts.panes.length > 1;
  container.classList.add('terminal-container');
  container.innerHTML = `
    <div class="flex flex-wrap items-center justify-between gap-2 px-4 py-2 bg-surface-900 border-t border-surface-700">
      <div class="flex items-center gap-2 min-w-0">
        <div class="hidden sm:flex gap-1.5">
          <div class="w-3 h-3 rounded-full bg-red-500"></div>
          <div class="w-3 h-3 rounded-full bg-yellow-500"></div>
          <div class="w-3 h-3 rounded-full bg-green-500"></div>
        </div>
        <span class="text-sm font-mono text-surface-300">${opts.title || 'Terminal'}</span>
        <span class="terminal-status text-xs text-surface-400"></span>
      </div>
      <div class="flex items-center gap-2">
        <button class="terminal-kill text-xs px-2 py-1 rounded text-red-300 hover:bg-surface-800 transition-colors hidden" title="Send Ctrl+C (SIGINT) to the running program">Ctrl+C</button>
        <button class="terminal-fs text-xs px-2 py-1 rounded text-surface-300 hover:bg-surface-800 transition-colors"><span class="fs-enter">Fullscreen</span><span class="fs-exit hidden">Exit fullscreen</span></button>
        <button class="terminal-close text-xs px-2 py-1 rounded text-surface-300 hover:bg-surface-800 transition-colors">Close</button>
      </div>
    </div>
    <div class="term-banner hidden flex flex-wrap items-center justify-between gap-2 px-4 py-2 bg-amber-950 text-amber-100 text-sm border-t border-amber-800">
      <span class="term-banner-text"></span>
      <button class="term-restart px-3 py-1 rounded-md bg-amber-500 text-surface-950 font-semibold text-xs hover:bg-amber-400">Restart</button>
    </div>
    <div class="term-panes grid ${multi ? 'md:grid-cols-2' : ''} gap-px bg-surface-700">
      ${opts.panes.map((p, i) => `
        <div class="term-pane flex flex-col bg-surface-950 min-w-0" data-pane="${i}">
          ${multi ? `<div class="px-3 py-1 text-xs font-mono text-surface-300 bg-surface-900 border-b border-surface-800">${p.label || `Terminal ${i + 1}`}</div>` : ''}
          <div class="terminal-target" style="height:${opts.height || 350}px;"></div>
          <div class="term-chips flex flex-wrap items-center gap-2 px-3 py-1.5 min-h-[34px] bg-surface-900 border-t border-surface-800"></div>
        </div>`).join('')}
    </div>`;

  const $ = <T extends HTMLElement>(sel: string) => container.querySelector(sel) as T;
  const statusEl = $('.terminal-status');
  const killBtn = $('.terminal-kill');
  const banner = $('.term-banner');
  let ws: WebSocket | null = null;
  let userClosed = false;
  let ended = false;

  const panes = opts.panes.map((plan, i) => {
    const el = container.querySelector(`.term-pane[data-pane="${i}"]`) as HTMLElement;
    const term = new Terminal({
      theme: { background: '#0f172a', foreground: '#e2e8f0', cursor: '#22d3ee', selectionBackground: '#334155' },
      fontFamily: "'JetBrains Mono', 'Fira Code', monospace",
      fontSize: fontSize(),
      lineHeight: 1.4,
      cursorBlink: true,
    });
    const fit = new FitAddon();
    term.loadAddon(fit);
    term.open(el.querySelector('.terminal-target') as HTMLElement);
    return {
      i, plan, el, term, fit,
      chips: el.querySelector('.term-chips') as HTMLElement,
      // boot -> compiling -> (waitTurn) -> running -> idle
      phase: (opts.mode === 'example' && (plan.compile || plan.run)) ? 'boot' : 'idle',
      opened: i === 0,
      started: false,
      ranCommand: false, // a command ran since the last prompt (the very first prompt gets no chip)
      stopTimer: 0 as any,
      interruptTimer: 0 as any,
      runOutput: '',           // what the program printed during this run (for challenge checks)
      oscPending: '',          // start of an escape sequence still waiting for its end
    };
  });
  type Pane = typeof panes[number];

  const send = (msg: object) => { if (ws && ws.readyState === 1) ws.send(JSON.stringify(msg)); };
  const type = (p: Pane, text: string) => send({ type: 'input', pane: p.i, data: text });

  function chip(p: Pane, text: string, tone: keyof typeof TONE, kind: string, extra?: HTMLElement) {
    p.chips.querySelectorAll(`.term-chip[data-kind="${kind}"]`).forEach((c) => c.remove());
    const c = document.createElement('span');
    c.className = `term-chip inline-flex items-center gap-2 px-2.5 py-0.5 rounded-full border text-xs font-medium ${TONE[tone]}`;
    c.dataset.kind = kind;
    c.textContent = text;
    if (extra) c.appendChild(extra);
    p.chips.appendChild(c);
  }
  const clearChips = (p: Pane, kind?: string) =>
    p.chips.querySelectorAll(kind ? `.term-chip[data-kind="${kind}"]` : '.term-chip').forEach((c) => c.remove());

  function updateKill() {
    killBtn.classList.toggle('hidden', !panes.some((p) => p.phase === 'running' || p.phase === 'compiling'));
  }

  function startRun(p: Pane) {
    p.phase = 'running';
    p.started = true;
    p.runOutput = '';
    type(p, `${runOf(p.plan)}\n`);
    container.dispatchEvent(new CustomEvent('term:run', { bubbles: true, detail: { pane: p.i } }));
    if (p.plan.interruptAfterMs) {
      p.interruptTimer = setTimeout(() => {
        if (p.phase !== 'running') return;
        chip(p, 'Sent Ctrl+C', 'info', 'interrupt');
        send({ type: 'signal', pane: p.i, signal: 'SIGINT' });
      }, p.plan.interruptAfterMs);
    }
    if (p.plan.stopAfterMs) {
      p.stopTimer = setTimeout(() => {
        if (p.phase !== 'running') return;
        const sig = p.plan.stopSignal || 'SIGINT';
        chip(p, `Stopped automatically after ${Math.round(p.plan.stopAfterMs! / 1000)}s (sent ${sig === 'SIGQUIT' ? 'Ctrl+\\' : 'Ctrl+C'})`, 'info', 'auto-stop');
        send({ type: 'signal', pane: p.i, signal: sig });
      }, p.plan.stopAfterMs);
    }
    // Programs that wait in select()/poll() (chat clients) never look like a keyboard read,
    // so the sample input is offered for as long as the program runs
    if (p.plan.sampleInput) chip(p, 'Sample input ready', 'info', 'sample', sampleButton(p));
    // Pair: the other programs start a moment after this one (server before client)
    if (p.i === 0) {
      for (const q of panes.slice(1)) if (q.phase === 'waitTurn') setTimeout(() => startRun(q), q.plan.startAfterMs ?? 800);
    }
    updateKill();
  }

  function onPrompt(p: Pane, code: number) {
    clearTimeout(p.interruptTimer);
    // Tell listeners (challenge checks) how a run or compile ended
    if (p.phase === 'compiling' && code !== 0) container.dispatchEvent(new CustomEvent('term:compile-failed', { bubbles: true, detail: { pane: p.i } }));
    if (p.phase === 'running') container.dispatchEvent(new CustomEvent('term:exit', { bubbles: true, detail: { pane: p.i, code, output: p.runOutput } }));
    clearChips(p, 'input');
    if (p.phase === 'running') clearChips(p, 'sample');
    clearTimeout(p.stopTimer);
    if (p.phase === 'boot') {
      if (p.plan.compile) { p.phase = 'compiling'; type(p, `${p.plan.compile}\n`); }
      else readyToRun(p);
    } else if (p.phase === 'compiling') {
      if (code !== 0) { p.phase = 'idle'; chip(p, 'Compile failed — see the errors above', 'bad', 'status'); }
      else readyToRun(p);
    } else {
      p.phase = 'idle';
      if (p.ranCommand) { const d = describeStatus(code); chip(p, d.text, d.tone, 'status'); }
    }
    p.ranCommand = false;
    updateKill();
  }

  function readyToRun(p: Pane) {
    if (p.i > 0 && !panes[0].started) { p.phase = 'waitTurn'; chip(p, `Waiting for ${panes[0].plan.label || 'the first program'} to start…`, 'info', 'status'); return; }
    if (p.i > 0) { p.phase = 'waitTurn'; setTimeout(() => startRun(p), p.plan.startAfterMs ?? 800); return; }
    startRun(p);
  }

  function sampleButton(p: Pane) {
    const btn = document.createElement('button');
    btn.className = 'term-sample underline underline-offset-2 hover:text-white';
    btn.textContent = 'Insert sample input';
    btn.title = p.plan.sampleInput!;
    btn.addEventListener('click', () => {
      type(p, p.plan.sampleInput!.endsWith('\n') ? p.plan.sampleInput! : `${p.plan.sampleInput}\n`);
      clearChips(p, 'sample');
      p.term.focus();
    });
    return btn;
  }

  function onInputWait(p: Pane, waiting: boolean) {
    if (!waiting) { clearChips(p, 'input'); return; }
    const offer = p.plan.sampleInput && p.chips.querySelector('.term-chip[data-kind="sample"]');
    if (offer) clearChips(p, 'sample');
    chip(p, 'Program is waiting for input — type and press Enter', 'info', 'input', offer ? sampleButton(p) : undefined);
  }

  function showBanner(text: string) {
    ended = true;
    banner.classList.remove('hidden');
    ($('.term-banner-text')).textContent = text;
    statusEl.textContent = 'Disconnected';
    killBtn.classList.add('hidden');
  }

  function fitAll() { for (const p of panes) { try { p.fit.fit(); } catch {} } }

  let ro: ResizeObserver | null = null;
  function teardown() {
    userClosed = true;
    try { ws?.close(); } catch {}
    ro?.disconnect();
    for (const p of panes) { clearTimeout(p.stopTimer); clearTimeout(p.interruptTimer); p.term.dispose(); }
  }

  ($('.term-restart')).addEventListener('click', () => { teardown(); openTerminal(container, opts); });
  ($('.terminal-close')).addEventListener('click', () => {
    if (container.classList.contains('is-fullscreen')) document.body.classList.remove('fs-lock');
    teardown();
    container.remove();
  });
  killBtn.addEventListener('click', () => {
    for (const p of panes) if (p.phase === 'running' || p.phase === 'compiling') send({ type: 'signal', pane: p.i, signal: 'SIGINT' });
  });

  setTimeout(() => {
    fitAll();
    const url = getTerminalUrl();
    if (!url) {
      panes[0].term.writeln('\x1b[1;31mTerminal server not configured.\x1b[0m');
      panes[0].term.writeln('Run: \x1b[33mlocalStorage.setItem("terminal-url","ws://localhost:8090")\x1b[0m then reload.');
      return;
    }
    for (const p of panes) p.term.writeln('\x1b[2mStarting Linux sandbox…\x1b[0m');
    statusEl.textContent = 'Connecting…';
    ws = new WebSocket(url);

    ws.onopen = () => {
      statusEl.textContent = 'Starting…';
      const t = panes[0].term;
      send({ type: 'start', files: opts.files, cols: t.cols, rows: t.rows, mode: opts.mode });
    };

    ws.onmessage = (event) => {
      const msg = JSON.parse(event.data);
      const p = panes[msg.pane ?? 0];
      if (msg.type === 'ready') {
        statusEl.textContent = 'Connected';
        for (const q of panes.slice(1)) send({ type: 'open-pane', pane: q.i, cols: q.term.cols, rows: q.term.rows });
        opts.onReady?.();
      } else if (msg.type === 'output' && p) {
        p.term.write(msg.data);
        const [complete, pending] = takeComplete(p.oscPending, msg.data);
        p.oscPending = pending;
        if (p.phase === 'running' && p.runOutput.length < 200_000) p.runOutput += complete.replace(OSC, '');
        for (const m of complete.matchAll(OSC)) {
          const [ev, arg] = m[1].split(';');
          if (ev === 'prompt') onPrompt(p, Number(arg));
          else if (ev === 'exec') { p.ranCommand = true; clearChips(p, 'status'); clearChips(p, 'auto-stop'); if (p.phase === 'idle') { p.phase = 'running'; updateKill(); } }
        }
      } else if (msg.type === 'input-wait' && p) {
        onInputWait(p, msg.waiting);
      } else if (msg.type === 'exit') {
        if ((msg.pane ?? 0) === 0) showBanner('The shell ended.');
        else if (p) chip(p, 'Shell ended', 'warn', 'status');
      } else if (msg.type === 'error') {
        panes[0].term.writeln(`\x1b[1;31m${msg.data}\x1b[0m`);
        statusEl.textContent = 'Error';
      }
    };

    ws.onclose = () => {
      if (userClosed) return;
      if (!ended) showBanner('Connection to the sandbox was lost (idle timeout, time limit or a server restart).');
    };
    ws.onerror = () => { statusEl.textContent = 'Error'; };

    for (const p of panes) {
      p.term.onData((data) => send({ type: 'input', pane: p.i, data }));
      p.term.onResize(({ cols, rows }) => send({ type: 'resize', pane: p.i, cols, rows }));
    }

    let fitTimeout: ReturnType<typeof setTimeout>;
    ro = new ResizeObserver(() => {
      clearTimeout(fitTimeout);
      fitTimeout = setTimeout(() => { const y = window.scrollY; fitAll(); window.scrollTo(0, y); }, 100);
    });
    for (const p of panes) ro.observe(p.el.querySelector('.terminal-target')!);
  }, 150);

  // For the presenter view and tests: change font size on the fly
  (container as any).__setFontSize = (n: number) => { for (const p of panes) { p.term.options.fontSize = n; } fitAll(); };
}
