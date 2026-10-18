# Lecture day — presenting from the live site

The site and every terminal run on Cloudflare. **Presenting needs only a browser** — nothing from this
repo has to run on the lecture laptop. The steps below are for setting the presenter password once and,
optionally, re-running the validation from your own laptop.

Site: https://cs-lectures.pages.dev · Presenter sign-in: https://cs-lectures.pages.dev/presenter/ ·
Status: https://cs-lectures.pages.dev/status/

## 0. First lecture: the intro deck

https://cs-lectures.pages.dev/intro/ — 13 animated slides (about 10–15 minutes): who I am, the course,
the semester roadmap, a **live `fork()` demo**, the website, how to succeed, contact, and C++ → C.

- `→` / `←`, Space, or a clicker's PageDown / PageUp step slides (PageDown works even while the demo
  terminal has focus); `F` = full screen; `Home` / `End`; `/intro/#9` opens a given slide.
- Slide 9 has a real **Run** button — sign in on `/presenter/` first so it runs in your sandbox.
- Slide 6 (the kitchen) reveals one card per click — the deck moves on after the eighth.
- The last slide's **Start Lesson 01 →** opens Lesson 01 straight in the presenter view.
- Checked at 1920×1080, 1366×768 and 1280×720 (nothing needs scrolling) and on phones (check V20).

## 1. On the lecture computer (every time, 1 minute)

1. Open https://cs-lectures.pages.dev/presenter/ and type the presenter password. That browser stays
   in presenter mode for 90 days: your examples run in your own reserved sandbox (never shared with
   students, never rate-limited) and the header shows a green **Presenter** badge.
2. Open the lesson ~2 minutes before you start — the badge wakes your sandbox in the background.
3. On the lesson page press **Present**:
   - `→` / `←` or a clicker's PageDown / PageUp — next / previous example
   - `Ctrl+Enter` — run the current example (Run pair for two-program examples)
   - `+` / `−` — bigger / smaller text (remembered) · `Esc` — leave
   - A link like `/os/02-basic-forks/?present=3` opens straight on example 3.
4. If the badge turns red ("sign in again"), the pass expired or the password changed — repeat step 1.
5. To leave presenter mode on a shared computer: the **Sign out** button on `/presenter/`.

If something hangs on stage: the terminal's **Ctrl+C** button, then Run again. A new terminal is always
a fresh sandbox.

## 2. Set or change the presenter password (once, from any laptop with Node 22)

```bash
git clone https://github.com/YanivGabay/cs-lectures-by-yaniv-gabay.git
cd cs-lectures-by-yaniv-gabay/terminal-server/cloudflare
npm install
npx wrangler login                          # opens the browser — log in to Cloudflare
npx wrangler secret put PRESENTER_PASSWORD  # type the password when asked (it is not shown)
```

Type it at the prompt — never put the password on the command line or in a file in the repo.
Changing it signs every presenter browser out; sign in again on `/presenter/`.

## 3. Re-run the validation from your laptop (optional, ~15 minutes)

Needs Node 22 and pnpm (`npm install -g pnpm`). Works on macOS, Windows and Linux; it only talks to the
live site, no Docker needed.

```bash
cd cs-lectures-by-yaniv-gabay/validate
pnpm install
npx playwright-core install --only-shell chromium   # the headless browser the checks drive

node site.mjs                                 # site: pages, dark mode, Run, chips, Run pair, presenter view…
TERMINAL_URL=wss://cs-lectures-terminal.yaniv242.workers.dev node security.mjs   # sandbox security
TERMINAL_URL=wss://cs-lectures-terminal.yaniv242.workers.dev node --experimental-strip-types examples.mjs  # all 114 examples
```

Every line prints `PASS`, `FAIL` (with the reason) or `SKIP` (could not run here, never counted as a pass)
and ends with `N/N passed`. The example audit writes `validate/reports/examples.md`; it should end with
`unexplained problems: 0`.

The presenter checks (V17, S17) need your password. Give it without it appearing on screen or in history:

```bash
# macOS / Linux
read -s PRESENTER_PASSWORD && export PRESENTER_PASSWORD && ONLY=V17,V17b node site.mjs
```

```powershell
# Windows PowerShell
$env:PRESENTER_PASSWORD = Read-Host -MaskInput "Presenter password"; $env:ONLY = "V17,V17b"; node site.mjs; Remove-Item Env:PRESENTER_PASSWORD, Env:ONLY
```

(On Windows set the other variables the same way, e.g. `$env:TERMINAL_URL = "wss://cs-lectures-terminal.yaniv242.workers.dev"`.)

Run them one after another, not at the same time: `security.mjs` floods the connection limit on purpose
(S13) and locks presenter logins for a minute (S19) — wait a minute before the next suite.

## Still to do

- **Billing alert ($10)** — Cloudflare dashboard → Notifications → Add → *Usage Based Billing* →
  Workers/Containers, threshold $10. Normal use is about $6/month in semester.
- **Review the course-code fixes** in `docs/course-fixes-for-review.md`.
- Open issues and limits: `KNOWN_ISSUES.md`.
