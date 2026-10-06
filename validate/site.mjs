// Browser-level validation of the CS Lectures site.
// BASE_URL=https://cs-lectures.pages.dev node site.mjs   (ONLY=V02,V05 to run a subset)
import { launch, check, assert, finish, stripAnsi, sleep, waitFor, Skip } from './lib.mjs';

// The lecturer's password is never stored in the repo; pass it in to run the presenter checks
const PRESENTER_PASSWORD = process.env.PRESENTER_PASSWORD || '';

const BASE = (process.env.BASE_URL || 'https://cs-lectures.pages.dev').replace(/\/$/, '');
const browser = await launch();

async function newPage({ scheme = 'light', theme = null, viewport = { width: 1400, height: 900 } } = {}) {
  const ctx = await browser.newContext({ colorScheme: scheme, viewport });
  if (theme) await ctx.addInitScript((t) => localStorage.setItem('theme', t), theme);
  const page = await ctx.newPage();
  page.errors = [];
  page.on('pageerror', (e) => page.errors.push(e.message));
  page.on('console', (m) => { if (m.type() === 'error') page.errors.push(m.text()); });
  // Capture terminal output straight from the WebSocket frames
  page.termOut = '';
  page.containers = []; // which container each terminal landed on (from the 'ready' message)
  page.on('websocket', (ws) => ws.on('framereceived', (f) => {
    try {
      const m = JSON.parse(f.payload);
      if (m.type === 'output' || m.type === 'error') page.termOut += m.data;
      if (m.type === 'ready') page.containers.push(m.container);
    } catch {}
  }));
  return page;
}

const termText = (page) => stripAnsi(page.termOut);
const waitTerm = (page, re, ms, what) =>
  waitFor(() => re.test(termText(page)), ms, what || String(re)).catch((e) => {
    throw new Error(`${e.message}; terminal ended with: ${JSON.stringify(termText(page).slice(-240))}`);
  });

async function typeInTerminal(page, slot, line) {
  await slot.locator('.xterm').click();
  await page.keyboard.type(line, { delay: 5 });
  await page.keyboard.press('Enter');
}

// Runs in the page: reports visible text whose contrast against its effective background is too low
function lowContrast(minRatio) {
  const cv = document.createElement('canvas');
  cv.width = cv.height = 1;
  const ctx = cv.getContext('2d', { willReadFrequently: true });
  const rgba = (c) => {
    ctx.clearRect(0, 0, 1, 1); ctx.fillStyle = '#000'; ctx.fillStyle = c; ctx.fillRect(0, 0, 1, 1);
    const d = ctx.getImageData(0, 0, 1, 1).data;
    return { r: d[0], g: d[1], b: d[2], a: d[3] / 255 };
  };
  const lum = ({ r, g, b }) => {
    const f = (v) => { v /= 255; return v <= 0.03928 ? v / 12.92 : Math.pow((v + 0.055) / 1.055, 2.4); };
    return 0.2126 * f(r) + 0.7152 * f(g) + 0.0722 * f(b);
  };
  const bgOf = (el) => {
    const layers = [];
    for (let e = el; e; e = e.parentElement) {
      const c = rgba(getComputedStyle(e).backgroundColor);
      if (c.a > 0) { layers.push(c); if (c.a >= 0.99) break; }
    }
    let base = { r: 255, g: 255, b: 255 };
    for (let i = layers.length - 1; i >= 0; i--) {
      const l = layers[i];
      base = { r: l.r * l.a + base.r * (1 - l.a), g: l.g * l.a + base.g * (1 - l.a), b: l.b * l.a + base.b * (1 - l.a) };
    }
    return base;
  };
  const skip = 'pre, code, svg, script, style, noscript, .xterm, .terminal-output, .code-block-wrapper';
  const bad = [];
  for (const el of document.querySelectorAll('body *')) {
    const own = [...el.childNodes].filter((n) => n.nodeType === 3).map((n) => n.textContent.trim()).join(' ').trim();
    if (!own || el.closest(skip)) continue;
    if (!el.checkVisibility({ opacityProperty: true, visibilityProperty: true })) continue;
    const fg = rgba(getComputedStyle(el).color);
    if (fg.a === 0) continue; // gradient text (bg-clip-text)
    const bg = bgOf(el);
    const fgMix = { r: fg.r * fg.a + bg.r * (1 - fg.a), g: fg.g * fg.a + bg.g * (1 - fg.a), b: fg.b * fg.a + bg.b * (1 - fg.a) };
    const [a, b] = [lum(fgMix), lum(bg)].sort((x, y) => y - x);
    const ratio = (a + 0.05) / (b + 0.05);
    if (ratio < minRatio) bad.push(`${ratio.toFixed(2)} "${own.slice(0, 40)}"`);
  }
  return bad;
}

function bodyState() {
  const cv = document.createElement('canvas'); cv.width = cv.height = 1;
  const ctx = cv.getContext('2d');
  ctx.fillStyle = getComputedStyle(document.body).backgroundColor; ctx.fillRect(0, 0, 1, 1);
  const [r, g, b] = ctx.getImageData(0, 0, 1, 1).data;
  return { cls: document.documentElement.classList.contains('dark'), darkBg: (0.299 * r + 0.587 * g + 0.114 * b) < 100 };
}

// Discover every page from the course overview
const discover = await newPage();
await discover.goto(`${BASE}/os/`);
const lessonPaths = [...new Set(await discover.$$eval('a[href^="/os/"]', (as) => as.map((a) => new URL(a.getAttribute('href'), location.href).pathname)))].filter((p) => p !== '/os/');
await discover.context().close();
const PAGES = ['/', '/os/', '/status/', '/presenter/', ...lessonPaths];

await check('V01', `all ${PAGES.length} pages load with no JS errors`, async () => {
  const page = await newPage();
  const problems = [];
  for (const p of PAGES) {
    page.errors.length = 0;
    const res = await page.goto(BASE + p, { waitUntil: 'load' });
    if (res.status() !== 200) problems.push(`${p} -> ${res.status()}`);
    await sleep(300);
    if (page.errors.length) problems.push(`${p}: ${page.errors[0].slice(0, 100)}`);
  }
  await page.context().close();
  assert(!problems.length, problems.join('; '));
});

await check('V02', 'dark/light toggle visibly changes the page (both OS preferences, persists on reload)', async () => {
  const out = [];
  for (const scheme of ['light', 'dark']) {
    for (const path of ['/', '/os/02-basic-forks/']) {
      const page = await newPage({ scheme });
      await page.goto(BASE + path);
      const s0 = await page.evaluate(bodyState);
      assert(s0.cls === s0.darkBg, `[${scheme} OS ${path}] initial class dark=${s0.cls} but background dark=${s0.darkBg}`);
      await page.click('#theme-toggle');
      await sleep(400);
      const s1 = await page.evaluate(bodyState);
      assert(s1.cls !== s0.cls, `[${scheme} OS ${path}] toggle did not flip the dark class`);
      assert(s1.darkBg === s1.cls, `[${scheme} OS ${path}] class flipped to dark=${s1.cls} but background stayed dark=${s1.darkBg}`);
      await page.reload();
      const s2 = await page.evaluate(bodyState);
      assert(s2.cls === s1.cls && s2.darkBg === s1.darkBg, `[${scheme} OS ${path}] choice not kept after reload`);
      out.push(`${scheme}:${path} ok`);
      await page.context().close();
    }
  }
  return out.join(', ');
});

await check('V03', 'readable text contrast (>= 3:1) on key pages in both themes', async () => {
  const problems = [];
  for (const theme of ['light', 'dark']) {
    for (const p of ['/', '/os/', '/os/02-basic-forks/', '/status/']) {
      const page = await newPage({ scheme: theme, theme });
      await page.goto(BASE + p);
      await sleep(1800); // let the stats counter finish
      const bad = await page.evaluate(lowContrast, 3);
      if (bad.length) problems.push(`[${theme} ${p}] ${bad.length} low: ${bad.slice(0, 4).join(' | ')}`);
      await page.context().close();
    }
  }
  assert(!problems.length, problems.join(' ;; '));
});

await check('V04', 'Run button compiles and runs an example (fork output appears)', async () => {
  const page = await newPage();
  await page.goto(`${BASE}/os/02-basic-forks/`);
  const block = page.locator('.code-block-wrapper', { has: page.locator('span', { hasText: /^01-basic-forking\.c$/ }) });
  await block.locator('.open-terminal-btn').click();
  await waitTerm(page, /\[Child\][\s\S]*\[Parent\]|\[Parent\][\s\S]*\[Child\]/, 40000, 'parent and child output');
  await page.context().close();
});

// One full-terminal session on lesson 10 (has files with the same name in different folders)
let full;
await check('V05', 'full terminal shows a welcome, lists every lesson file (with folders), and is not root', async () => {
  full = await newPage();
  await full.goto(`${BASE}/os/10-shared-memory/`);
  await full.click('.open-full-terminal-btn');
  const slot = full.locator('.full-terminal-slot');
  full.slot = slot;
  await waitTerm(full, /[$#] $/m, 40000, 'a shell prompt');
  await sleep(500);
  const t = termText(full);
  assert(/CS Lectures sandbox/.test(t), 'no welcome banner — student sees a bare prompt');
  assert(/02-basic-example\/consumer\.c/.test(t) && /04-array\/consumer\.c/.test(t), 'file list missing or same-named files collide');
  await typeInTerminal(full, slot, 'echo UID_$(id -u)_END');
  await waitTerm(full, /UID_\d+_END/, 8000, 'uid echo');
  const uid = Number(termText(full).match(/UID_(\d+)_END/)[1]);
  assert(uid !== 0, 'shell runs as root (uid 0)');
});

await check('V06', 'full terminal can compile a lesson file', async () => {
  assert(full, 'V05 did not open a terminal');
  await typeInTerminal(full, full.slot, 'gcc -o t 02-basic-example/creator.c && echo BUILD_$((1+1))_OK');
  await waitTerm(full, /BUILD_2_OK/, 30000, 'compile success marker');
});

await check('V07', 'Ctrl+C button interrupts a running program', async () => {
  assert(full, 'V05 did not open a terminal');
  await typeInTerminal(full, full.slot, 'sleep 100');
  await sleep(1500);
  await full.slot.locator('.terminal-kill').click();
  await sleep(300);
  await typeInTerminal(full, full.slot, 'echo INT_$((40+2))');
  await waitTerm(full, /INT_42/, 6000, 'shell to come back after Ctrl+C');
});
if (full) await full.context().close();

// ---- exit-status events and chips (T08/T09) ----
const chipText = (scope) => scope.locator('.term-chip').allInnerTexts().then((t) => t.join(' | '));
const waitChip = (scope, re, ms, what) =>
  waitFor(async () => re.test(await chipText(scope)), ms, what).catch(async (e) => {
    throw new Error(`${e.message}; chips: ${JSON.stringify(await chipText(scope))}`);
  });
const oscStatuses = (page) => [...page.termOut.matchAll(/\x1b\]777;cs;prompt;(\d+)\x07/g)].map((m) => Number(m[1]));

// Replace an example's code through the editor, then press Run
async function runEdited(page, file, code) {
  const block = page.locator('.code-block-wrapper', { has: page.locator('span', { hasText: new RegExp(`^${file.replace('.', '\\.')}$`) }) });
  await block.locator('.edit-btn').click();
  await block.locator('.CodeMirror').waitFor({ timeout: 20000 });
  await block.evaluate((el, c) => el.querySelector('.CodeMirror').CodeMirror.setValue(c), code);
  await block.locator('.open-terminal-btn').click();
  return block;
}

await check('V10', 'the shell reports every command\'s exit status (0, 1, SIGSEGV=139) and the chip names it', async () => {
  const page = await newPage();
  await page.goto(`${BASE}/os/02-basic-forks/`);
  await page.click('.open-full-terminal-btn');
  const slot = page.locator('.full-terminal-slot');
  await waitTerm(page, /[$#] $/m, 40000, 'a shell prompt');
  assert(!(await chipText(slot)), 'a status chip appeared before any command ran');
  await typeInTerminal(page, slot, 'true');
  await waitChip(slot, /Exited 0/, 8000, 'Exited 0 chip');
  await typeInTerminal(page, slot, 'false');
  await waitChip(slot, /Exited 1/, 8000, 'Exited 1 chip');
  await typeInTerminal(page, slot, "sh -c 'kill -SEGV $$'");
  await waitChip(slot, /Killed by SIGSEGV \(11\) — invalid memory access/, 8000, 'SIGSEGV chip');
  const st = oscStatuses(page);
  await page.context().close();
  assert(st.slice(-3).join(',') === '0,1,139', `prompt events carried ${st.join(',')}`);
});

await check('V11', 'a program that segfaults shows "Killed by SIGSEGV" under the Run terminal', async () => {
  const page = await newPage();
  await page.goto(`${BASE}/os/02-basic-forks/`);
  const block = await runEdited(page, '01-basic-forking.c', '#include <stdio.h>\nint main(void) {\n  int *p = NULL;\n  printf("about to crash\\n");\n  fflush(stdout);\n  return *p;\n}\n');
  await waitChip(block, /Killed by SIGSEGV/, 40000, 'SIGSEGV chip');
  await waitTerm(page, /about to crash/, 1000, 'program output before the crash');
  await page.context().close();
});

await check('V12', 'broken code shows "Compile failed" and is not run', async () => {
  const page = await newPage();
  await page.goto(`${BASE}/os/02-basic-forks/`);
  const block = await runEdited(page, '01-basic-forking.c', 'int main(void) { this is not C }\n');
  await waitChip(block, /Compile failed/, 40000, 'Compile failed chip');
  await sleep(800);
  assert(!/Exited|Killed/.test(await chipText(block)), 'the broken program was still run');
  await page.context().close();
});

await check('V13', 'when the session ends the terminal says so and Restart opens a fresh one', async () => {
  const page = await newPage();
  await page.goto(`${BASE}/os/02-basic-forks/`);
  await page.click('.open-full-terminal-btn');
  const slot = page.locator('.full-terminal-slot');
  await waitTerm(page, /[$#] $/m, 40000, 'a shell prompt');
  await typeInTerminal(page, slot, 'exit');
  await slot.locator('.term-banner').waitFor({ state: 'visible', timeout: 8000 });
  const before = page.containers.length;
  page.termOut = '';
  await slot.locator('.term-restart').click();
  await waitFor(() => page.containers.length > before, 40000, 'a new session after Restart');
  await waitTerm(page, /CS Lectures sandbox/, 15000, 'welcome banner in the restarted terminal');
  assert(!(await slot.locator('.term-banner').isVisible()), 'banner still showing after Restart');
  assert(await slot.locator('.terminal-container').count() === 1, 'Restart left more than one terminal');
  await page.context().close();
});

await check('V14', 'Run pair: echo server + client talk in split panes; FIFO reader + writer exchange a message', async () => {
  const page = await newPage();
  await page.goto(`${BASE}/os/11-sockets/`);
  const server = page.locator('.code-block-wrapper', { has: page.locator('span', { hasText: /^01-example\/echo-server\.c$/ }) });
  await server.locator('.run-pair-btn').click();
  const panes = server.locator('.term-pane');
  await waitFor(async () => (await panes.count()) === 2, 10000, 'two panes');
  const paneText = async (i) => (await panes.nth(i).locator('.xterm-rows').innerText()).replace(/\n(?!\[)/g, '');
  await waitFor(async () => /\[Server\][^\n]*Received/.test(await paneText(0)), 60000, 'server receiving in the left pane').catch(async (e) => {
    throw new Error(`${e.message}; left: ${JSON.stringify((await paneText(0)).slice(-200))} right: ${JSON.stringify((await paneText(1)).slice(-200))}`);
  });
  await waitFor(async () => /\[Client\][^\n]*Received echo/.test(await paneText(1)), 30000, 'client echo in the right pane');

  await page.goto(`${BASE}/os/09-named-pipes-msg-queues/`);
  const reader = page.locator('.code-block-wrapper', { has: page.locator('span', { hasText: /^02-mkfifo-example\/fifo-reader\.c$/ }) });
  await reader.locator('.run-pair-btn').click();
  const fp = reader.locator('.term-pane');
  await waitFor(async () => (await fp.count()) === 2, 10000, 'two FIFO panes');
  await fp.nth(1).locator('.term-sample').click({ timeout: 60000 });
  // narrow panes wrap long lines, so compare without line breaks
  await waitFor(async () => /hello through the FIFO/.test((await fp.nth(0).locator('.xterm-rows').innerText()).replace(/\n/g, '')), 20000, 'reader shows the writer\'s message').catch(async (e) => {
    throw new Error(`${e.message}; reader: ${JSON.stringify((await fp.nth(0).locator('.xterm-rows').innerText()).slice(-240))}`);
  });
  await page.context().close();
});

await check('V15', 'an input-reading program shows "waiting for input" and Insert sample input feeds it', async () => {
  const page = await newPage();
  await page.goto(`${BASE}/os/01-intro-to-c/`);
  const block = page.locator('.code-block-wrapper', { has: page.locator('span', { hasText: /^01-basic-input-output\.c$/ }) });
  await block.locator('.open-terminal-btn').click();
  await waitChip(block, /waiting for input/, 60000, 'waiting-for-input chip');
  await block.locator('.term-sample').click();
  await waitChip(block, /Exited 0/, 20000, 'program finishing after the sample input');
  assert(!/waiting for input/.test(await chipText(block)), 'input chip still showing after the program ended');
  await page.context().close();
});

await check('V16', 'presenter view: arrows/PageDown step, Ctrl+Enter runs, font size survives a reload', async () => {
  const page = await newPage();
  await page.goto(`${BASE}/os/02-basic-forks/`);
  await page.click('.present-btn');
  const pos = page.locator('#present-bar .pm-pos');
  const title = () => page.locator('#present-bar .pm-title').innerText();
  await waitFor(async () => (await pos.innerText()).startsWith('1 /'), 5000, 'presenter on example 1');
  const first = await title();
  await page.keyboard.press('ArrowRight');
  await page.keyboard.press('PageDown');
  await waitFor(async () => (await pos.innerText()).startsWith('3 /'), 3000, 'step to example 3');
  await page.keyboard.press('ArrowLeft');
  await page.keyboard.press('PageUp');
  await waitFor(async () => (await pos.innerText()).startsWith('1 /'), 3000, 'back to example 1');
  assert(await title() === first, 'title changed after stepping back');
  const fs = page.locator('.code-block-wrapper.is-fullscreen');
  assert(await fs.count() === 1, 'current example is not full screen');
  const box = await fs.boundingBox();
  assert(box.height > 800, `example does not fill the projector (${box.height}px)`);
  await page.keyboard.press('Control+Enter'); // example 1 is a reference file: nothing to run
  await waitFor(async () => (await page.locator('#present-bar .pm-run').innerText()).includes('Nothing to run'), 3000, '"Nothing to run" hint');
  await page.keyboard.press('ArrowRight'); // 01-basic-forking.c
  await page.keyboard.press('Control+Enter');
  await waitTerm(page, /\[Child\][\s\S]*\[Parent\]|\[Parent\][\s\S]*\[Child\]/, 60000, 'output after Ctrl+Enter');
  const before = await page.evaluate(() => getComputedStyle(document.querySelector('.is-fullscreen .code-scroll pre')).fontSize);
  await page.locator('#present-bar').click({ position: { x: 600, y: 20 } }); // keyboard focus back on the page
  await page.keyboard.press('+');
  await page.keyboard.press('+');
  const bigger = await page.evaluate(() => getComputedStyle(document.querySelector('.is-fullscreen .code-scroll pre')).fontSize);
  assert(parseFloat(bigger) === parseFloat(before) + 4, `+ + changed font ${before} -> ${bigger}`);
  assert(page.url().includes('present=2'), `deep link not in the address bar (${page.url()})`);
  await page.reload();
  await waitFor(async () => (await pos.innerText()).startsWith('2 /'), 5000, 'presenter restored from the link');
  const after = await page.evaluate(() => getComputedStyle(document.querySelector('.is-fullscreen .code-scroll pre')).fontSize);
  assert(after === bigger, `font size after reload ${after}, expected ${bigger}`);
  // The Bug vs Fix comparison is a slide too, and runs
  await page.keyboard.press('ArrowRight');
  await page.keyboard.press('ArrowRight');
  await waitFor(async () => (await page.locator('.diff-view.is-fullscreen').count()) === 1, 3000, 'Bug vs Fix slide');
  page.termOut = '';
  await page.keyboard.press('Control+Enter');
  await waitFor(() => page.termOut.includes('cs;prompt'), 60000, 'the bug side to run');
  await page.keyboard.press('Escape');
  assert(await page.locator('.is-fullscreen').count() === 0, 'Esc did not leave the presenter view');
  await page.context().close();
});

await check('V18', 'challenges: the starter gets a hint, each reference solution is judged "Challenge complete"', async () => {
  const fs = await import('node:fs');
  const solution = (f) => fs.readFileSync(new URL(`./challenge-solutions/${f}`, import.meta.url), 'utf8');
  const page = await newPage();
  const result = (card) => card.locator('.challenge-result');
  const runChallenge = async (card, code) => {
    if (code !== null) {
      if (!(await card.locator('.CodeMirror').count())) await card.locator('.edit-btn').click();
      await card.locator('.CodeMirror').waitFor({ timeout: 20000 });
      await card.evaluate((el, c) => el.querySelector('.CodeMirror').CodeMirror.setValue(c), code);
    }
    await result(card).evaluate((el) => { el.dataset.state = ''; }); // forget the previous run's verdict
    await card.locator('.open-terminal-btn').click();
    await waitFor(async () => /ok|bad/.test((await result(card).getAttribute('data-state')) || ''), 60000, 'a verdict');
    return result(card).innerText();
  };

  await page.goto(`${BASE}/os/08-pipes/`);
  const pipe = page.locator('#challenge-sigpipe');
  const hint = await runChallenge(pipe, null);
  assert(/Exited 0/.test(hint) && /Close fd\[0\]/.test(hint), `starter verdict: ${hint}`);
  const done = [];
  const cases = [['/os/08-pipes/', 'sigpipe', 'sigpipe.c'], ['/os/02-basic-forks/', 'fork-limit', 'fork-limit.c'],
    ['/os/06-sigaction/', 'survive-ctrl-c', 'survive-ctrl-c.c'], ['/os/14-cool-pthreads/', 'deadlock', 'deadlock.c']];
  for (const [url, id, file] of cases) {
    if (!page.url().endsWith(url)) await page.goto(`${BASE}${url}`);
    const card = page.locator(`#challenge-${id}`);
    const verdict = await runChallenge(card, solution(file));
    assert(/Challenge complete/.test(verdict), `${id} solution judged: ${verdict}`);
    assert(await card.locator('.challenge-done').isVisible(), `${id}: no Completed badge`);
    done.push(`${id}: ${verdict.split('.')[0]}`);
  }
  await page.context().close();
  return done.join(' · ');
});

await check('V08', 'long file "Show all" bar and fullscreen work (Esc exits)', async () => {
  const page = await newPage();
  await page.goto(`${BASE}/os/01-intro-to-c/`);
  const first = page.locator('.code-block-wrapper').first();
  const h0 = (await first.locator('.code-scroll').boundingBox()).height;
  await first.locator('.expand-btn').click();
  await sleep(200);
  const h1 = (await first.locator('.code-scroll').boundingBox()).height;
  assert(h1 > h0 + 200, `Show all did not grow the code (${h0} -> ${h1})`);
  await first.locator('.fs-btn').click();
  await sleep(200);
  const box = await first.boundingBox();
  assert(box.width >= 1390 && box.height >= 890, 'fullscreen does not fill the window');
  await page.keyboard.press('Escape');
  await sleep(200);
  assert(!(await first.getAttribute('class')).includes('is-fullscreen'), 'Esc did not exit fullscreen');
  await page.context().close();
});

await check('V09', `no horizontal scrolling on a 390px phone (all ${PAGES.length} pages)`, async () => {
  const problems = [];
  const page = await newPage({ viewport: { width: 390, height: 844 } });
  for (const p of PAGES) {
    await page.goto(BASE + p);
    const w = await page.evaluate(() => document.documentElement.scrollWidth);
    if (w > 391) problems.push(`${p} is ${w}px wide`);
  }
  await page.context().close();
  assert(!problems.length, problems.join('; '));
});

const runForkExample = async (page) => {
  await page.locator('.code-block-wrapper', { has: page.locator('span', { hasText: /^01-basic-forking\.c$/ }) }).locator('.open-terminal-btn').click();
  await waitFor(() => page.containers.length > 0, 40000, 'terminal ready');
  return page.containers[page.containers.length - 1];
};

await check('V19', 'status page lists every container without waking it; a warm Run reaches its prompt in < 3s', async () => {
  const page = await newPage();
  if (PRESENTER_PASSWORD) {
    await page.goto(`${BASE}/presenter/`);
    await page.fill('#pm-password', PRESENTER_PASSWORD);
    await page.click('#pm-submit');
    await page.locator('#pm-signed-in').waitFor({ state: 'visible', timeout: 15000 });
  }
  await page.goto(`${BASE}/status/`);
  await waitFor(async () => (await page.locator('#container-list li').count()) > 0, 20000, 'container rows');
  const rows = await page.locator('#container-list li').evaluateAll((els) => els.map((e) => `${e.id}=${e.dataset.state}`));
  const prod = rows.some((r) => r.startsWith('container-pool-'));
  if (prod) for (const n of ['pool-0', 'pool-1', 'pool-2', 'presenter']) assert(rows.some((r) => r.startsWith(`container-${n}=`)), `no row for ${n}: ${rows}`);
  const bad = rows.filter((r) => /=(unknown|undefined)$/.test(r));
  assert(!bad.length, `unreadable containers: ${bad}`);

  // Warm Run: open one terminal to wake the sandbox, then time a second Run from click to prompt
  await page.goto(`${BASE}/os/02-basic-forks/`);
  const block = page.locator('.code-block-wrapper', { has: page.locator('span', { hasText: /^01-basic-forking\.c$/ }) });
  await block.locator('.open-terminal-btn').click();
  await waitFor(() => page.termOut.includes('cs;prompt'), 60000, 'first (cold) terminal');
  await sleep(1000);
  page.termOut = '';
  const t0 = Date.now();
  await block.locator('.open-terminal-btn').click();
  await waitFor(() => page.termOut.includes('cs;prompt'), 20000, 'warm terminal prompt');
  const ms = Date.now() - t0;
  const landed = page.containers.at(-1);
  if (PRESENTER_PASSWORD && prod) {
    await page.goto(`${BASE}/status/`);
    await waitFor(async () => (await page.locator('#container-presenter').getAttribute('data-state')) === 'awake', 20000, 'presenter shown awake');
  }
  await page.context().close();
  assert(ms < 3000, `warm Run took ${ms}ms to reach the prompt on ${landed}`);
  return `${rows.join(' ')} · warm Run → prompt ${ms}ms on ${landed}`;
});

await check('V17', 'presenter sign-in on /presenter sticks across pages and reloads and reaches the reserved sandbox', async () => {
  if (!PRESENTER_PASSWORD) throw new Skip('set PRESENTER_PASSWORD to run');
  const page = await newPage();
  await page.goto(`${BASE}/presenter/`);
  await page.fill('#pm-password', PRESENTER_PASSWORD);
  await page.click('#pm-submit');
  await page.locator('#pm-signed-in').waitFor({ state: 'visible', timeout: 15000 });
  const stored = await page.evaluate(() => JSON.stringify(localStorage));
  assert(!stored.includes(PRESENTER_PASSWORD), 'the password itself was stored in the browser');
  assert(!page.url().includes(PRESENTER_PASSWORD), 'the password appears in the address bar');
  await page.goto(`${BASE}/os/02-basic-forks/`);
  await page.reload();
  await sleep(1500);
  assert(await page.locator('#presenter-badge').isVisible(), 'no Presenter badge after navigating and reloading');
  assert((await page.locator('#presenter-badge').innerText()).trim() === 'Presenter', 'badge does not say Presenter (pass rejected?)');
  const landed = await runForkExample(page);
  await page.context().close();
  assert(landed === 'presenter', `terminal landed on ${landed}`);
});

await check('V17b', 'a wrong password is refused and stores nothing; students land in the pool', async () => {
  const page = await newPage();
  await page.goto(`${BASE}/presenter/`);
  // Presenter login lives in the Cloudflare Worker; a bare local container has no such route
  const loginStatus = await page.evaluate(async () => {
    const api = document.querySelector('meta[name="terminal-url"]').content.replace(/^ws/, 'http');
    try { return (await fetch(`${api}/presenter/login`, { method: 'POST', body: '{}' })).status; } catch { return 0; }
  });
  if (loginStatus === 404 || loginStatus === 0) { await page.context().close(); throw new Skip('terminal server has no presenter login (local container, not the Worker)'); }
  await page.fill('#pm-password', 'definitely-not-the-password');
  await page.click('#pm-submit');
  await waitFor(async () => /Wrong password|Too many attempts/.test(await page.locator('#pm-error').innerText()), 15000, 'error message');
  assert(!(await page.evaluate(() => localStorage.getItem('presenterKey'))), 'a pass was stored after a wrong password');
  await page.goto(`${BASE}/os/02-basic-forks/`);
  assert(!(await page.locator('#presenter-badge').isVisible()), 'badge visible without signing in');
  const landed = await runForkExample(page);
  await page.context().close();
  assert(String(landed).startsWith('pool-'), `unsigned browser landed on ${landed}`);
});

await browser.close();
finish();
