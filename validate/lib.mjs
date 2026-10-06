import { chromium } from 'playwright-core';
import fs from 'node:fs';
import path from 'node:path';
import os from 'node:os';

export function chromePath() {
  if (process.env.CHROME_PATH) return process.env.CHROME_PATH;
  const base = path.join(os.homedir(), '.cache/ms-playwright');
  const dirs = fs.existsSync(base)
    ? fs.readdirSync(base).filter((d) => d.startsWith('chromium_headless_shell-')).sort().reverse()
    : [];
  for (const d of dirs) {
    const p = path.join(base, d, 'chrome-headless-shell-linux64/chrome-headless-shell');
    if (fs.existsSync(p)) return p;
  }
  throw new Error('No headless Chrome found; set CHROME_PATH');
}

export const launch = () => chromium.launch({ executablePath: chromePath() });

// ONLY=V02,S3 runs a subset
const only = (process.env.ONLY || '').split(',').map((s) => s.trim()).filter(Boolean);
const results = [];

// Throw this when a check cannot run here (e.g. no test password) — reported, never counted as a pass
export class Skip extends Error {}

export async function check(id, name, fn) {
  if (only.length && !only.includes(id)) return;
  try {
    const detail = await fn();
    results.push({ id, name, ok: true });
    console.log(`PASS ${id} ${name}${detail ? ` — ${detail}` : ''}`);
  } catch (e) {
    if (e instanceof Skip) { console.log(`SKIP ${id} ${name} — ${e.message}`); return; }
    results.push({ id, name, ok: false });
    console.log(`FAIL ${id} ${name} — ${e.message}`);
  }
}

export function assert(cond, msg) {
  if (!cond) throw new Error(msg);
}

export function finish() {
  const failed = results.filter((r) => !r.ok);
  console.log(`\n${results.length - failed.length}/${results.length} passed`);
  process.exitCode = failed.length ? 1 : 0;
}

export const stripAnsi = (s) => s.replace(/\x1b\][^\x07\x1b]*(\x07|\x1b\\)/g, '').replace(/\x1b\[[0-9;?]*[ -\/]*[@-~]/g, '').replace(/\x1b[()][0-9A-Za-z]/g, '').replace(/\r/g, '');

export const sleep = (ms) => new Promise((r) => setTimeout(r, ms));

export async function waitFor(fn, timeoutMs, what) {
  const end = Date.now() + timeoutMs;
  while (Date.now() < end) {
    if (await fn()) return;
    await sleep(150);
  }
  throw new Error(`timed out after ${timeoutMs}ms waiting for ${what}`);
}
