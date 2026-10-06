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
const PAGES = ['/', '/os/', '/status/', ...lessonPaths];

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

await check('V09', 'no horizontal scrolling on a 390px phone', async () => {
  const problems = [];
  for (const p of ['/', '/os/', '/os/08-pipes/', '/status/']) {
    const page = await newPage({ viewport: { width: 390, height: 844 } });
    await page.goto(BASE + p);
    const w = await page.evaluate(() => document.documentElement.scrollWidth);
    if (w > 391) problems.push(`${p} is ${w}px wide`);
    await page.context().close();
  }
  assert(!problems.length, problems.join('; '));
});

const runForkExample = async (page) => {
  await page.locator('.code-block-wrapper', { has: page.locator('span', { hasText: /^01-basic-forking\.c$/ }) }).locator('.open-terminal-btn').click();
  await waitFor(() => page.containers.length > 0, 40000, 'terminal ready');
  return page.containers[page.containers.length - 1];
};

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
