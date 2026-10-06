# AI in the in-browser C terminal — options, costs, risks

Prices were fetched live from `openrouter.ai/api/v1/models` on 2026-10-06. Jev pricing is from the TypeSafe skill.

## Constraints from the current architecture

- **Flow:** browser (xterm.js, `CodeBlock.astro`) → Worker (`terminal-server/cloudflare/src/index.ts`) → `container.fetch(request)` → Cloudflare Container running node-pty bash (`server.js`).
- **The Worker never sees terminal traffic.** It hands the WebSocket straight through to the container, so it doesn't see any frames.
- **The container has no internet** (`enableInternet = false`). AI calls cannot be made from the container.
- **Where AI calls must live:** in a Worker route, with `OPENROUTER_API_KEY` stored as a Worker secret (`wrangler secret put`). The key must never be in the browser bundle or the container image.
- **What the browser already has:** the source code (original or CodeMirror-edited) and the full output stream (`ws.onmessage` `output` frames). Most helpers can therefore be browser → Worker HTTP calls, with no change to the terminal protocol.
- **What the container sends:** raw keystrokes (`input` frames), not lines. Arrow keys, history, tab completion and pastes mean a "line" can only be approximated outside bash.

## Models (current prices, $ per 1M tokens)

| Model | In | Out | Use |
|---|---|---|---|
| `typesafe/jev-1.13` (`/api/alpha/decisions`) | 0.042 | 0 | Classification and scores, about 400 ms |
| `google/gemini-2.5-flash-lite` | 0.10 | 0.40 | Default for hints: fast, cheap, 1M context |
| `qwen/qwen3-coder-30b-a3b-instruct` | 0.07 | 0.28 | Code-focused fallback |
| `openai/gpt-4.1-nano` | 0.10 | 0.40 | Alternative non-reasoning nano |
| `openai/gpt-5-nano` | 0.05 | 0.40 | Reasoning model: hidden reasoning tokens make cost and latency less predictable |
| `google/gemini-2.5-flash` | 0.30 | 2.50 | Upgrade if lite hints are weak |

A typical hint call is about 3,100 input tokens (≈2.5K source, 300 error, 300 prompt) and about 150 output tokens. On gemini-2.5-flash-lite that is about **$0.0004 per call, or about $0.40 per 1,000 uses**.

---

## 1. Command gate with Jev (allow / warn / block)

**Position:** defense-in-depth and teaching telemetry only. The real controls are OS-level: a non-root user per session, `ulimit -u/-t/-v/-f`, killing the process group on disconnect, and no network. A classifier must never be the only barrier.

**Why it can't be a hard gate:**
- **Evasion is trivial.**
  - `echo OnwpezooOnw...|base64 -d|sh`
  - Writing the payload with `nano x.sh` and running `sh x.sh`
  - A shell alias, `eval`, or history expansion
  - Most importantly, compiling C that calls `fork()` in a loop. Jev sees `./a.out`, which is harmless text.
- **False positives are the curriculum.** `fork`, `kill`, `signal`, `pthread_create`, `mkfifo`, `ipcrm` and `kill -USR1 $pid` are all legitimate in this course. `kill -9 -1` is genuinely harmful, while `kill -9 1234` is homework.
- **Inline blocking adds about 400 ms per Enter** and needs the Worker to terminate and re-proxy the WebSocket (`WebSocketPair`). It also can't distinguish a bash command line from `scanf` input to a running program; only the container knows the PTY's foreground process group.

**Recommended shape, if built: async observe-and-flag.**
1. The Worker proxies the WebSocket itself instead of returning `container.fetch` directly.
2. It reconstructs lines from printable characters and backspace, flushing on `\r`.
3. It forwards every keystroke immediately, never waiting on the classifier.
4. It calls Jev via `ctx.waitUntil()` with one call carrying several questions:
   - `risk` Choice: `normal_course_work | resource_abuse | destructive | network_or_exfil`
   - `confidence_harm` Noul: "This command would harm other users or the host rather than the student's own program."
5. On a high score, it sends a `{type:'notice'}` frame ("this will hit the sandbox limits") and logs. It only ends the session for repeated `destructive` scores above 0.95.

| | |
|---|---|
| Runs in | Worker (WebSocket proxy) |
| Cost | ~400 tokens per line → **≈$0.017 per 1,000 commands** (negligible) |
| Latency | 0 if async; +400 ms if inline (not recommended) |
| Controls | Per-IP `ratelimits` binding; only classify lines over 3 chars; skip while a program is running if the container reports it |
| Privacy | Command lines only, no source code |
| Size | **M**: `src/index.ts` (WS proxy), new `src/gate.ts`, `wrangler.jsonc` |

---

## 2. "Explain this error" — hints, not answers

**Trigger (browser):** the output stream matches `error:` or `warning:` from gcc, `Segmentation fault`, `core dumped`, `undefined reference`, or `Aborted`. A small chip appears under the terminal: **💡 Explain this error**. Nothing is sent until the student clicks.

**Flow:**
1. The browser sends `POST /ai/hint {kind:'compile'|'runtime', file, source≤12KB, output_tail≤4KB}` to the Worker.
2. The Worker checks the Origin allowlist, applies a per-IP rate limit, and checks a global daily budget counter.
3. It looks up the Cache API by `sha256(kind+source+output)`.
4. On a miss, it calls OpenRouter chat with `max_tokens: 200`, `temperature: 0.2`.
5. **System prompt:** "You are a TA for an OS/C course. Give one hint, max 3 sentences, point to the line number. Do NOT write corrected code." The student's source goes in a delimited user block.

**Deterministic rules first, at $0 and 0 ms.** These cover most real errors before any model call:

| Pattern | Hint |
|---|---|
| ``undefined reference to `pthread_create'`` | Add `-lpthread` |
| `implicit declaration of function 'fork'` | Add `#include <unistd.h>` |
| `implicit declaration of function 'wait'` | Add `<sys/wait.h>` |
| `Permission denied` on `prog` | Run it as `./prog` |
| `Segmentation fault` with `scanf("%d", x)` | Missing `&` |

| | |
|---|---|
| Runs in | Browser (detect + UI), Worker (`/ai/hint`) |
| Model | gemini-2.5-flash-lite; fall back to qwen3-coder-30b on error |
| Cost | **≈$0.40 per 1,000 hints**; a cache hit is $0 (many students hit the same example error) |
| Latency | About 1 s |
| Controls | Opt-in click; 10 hints / 60 s / IP; daily budget stop in KV; source and output truncation; spending limit on the OpenRouter key itself as the final backstop |
| Privacy | Student code goes to OpenRouter and the upstream provider. Send `provider: {data_collection: "deny"}` and show a one-line notice on the chip. No names or accounts exist in our system; don't forward IPs. |
| Prompt injection | Low impact: a student injecting into their own hint only cheats themselves. Keep the "no corrected code" rule in the system prompt and cap output. |
| Size | **M**: `CodeBlock.astro` (detector, chip, render), new `terminal-server/cloudflare/src/ai.ts`, `src/index.ts` (route + CORS), `wrangler.jsonc` (ratelimit + KV) |

---

## 3. Stuck-on-input helper

**Detection (browser, no AI):** a program is running (autorun sent `./prog`) and the last output chunk has no trailing newline, or ends in `:`, `?`, `>` or `)`. Nothing has arrived for 4 s and the student hasn't typed. When that holds, show the chip **"Waiting for input? What does it expect?"**.

**AI part:** reuse `/ai/hint` with `kind:'stdin'`. The prompt asks the model to say what input the program reads at the current prompt, give the format, and give one example value, in 2 sentences, using the source and the last output line. This complements the `printf` prompts already added before every `scanf`. Its main value is the `%d %d`, `%s` and scanset cases.

| Cost | Latency | Size |
|---|---|---|
| ≈$0.35 per 1,000; heavily cacheable per example | About 1 s | **S**, once #2 exists (`CodeBlock.astro` only) |

---

## 4. Output explainer — "why is the order different?"

- **Runtime version:** a button after fork or thread examples sends the source plus the observed output to `/ai/hint` with `kind:'order'`. About $0.40 per 1,000.
- **Better version: precompute at build time.** For each of the 57 captured programs, generate a "What to notice" note once with gemini-2.5-flash. Store it in `site/src/data/explanations.json` and render it under Expected Output.
  - Total cost is about **$0.10 one-off**.
  - There's no runtime cost, no privacy issue and no latency.
  - **Yaniv reviews every note before it ships**, which matters for teaching accuracy.
  - The runtime button is then only needed for student-edited code.
- **Size:** S. A script in `site/scripts/`, plus `[lesson].astro` and `CodeBlock.astro`.

---

## 5. Other high-value ideas

| Idea | How | Cost | Size |
|---|---|---|---|
| **Common-mistake analytics for the teacher** | Jev classifies each hint request's error into ~15 buckets (missing header, link flag, segfault, uninitialized var, wrong format specifier…). Only counts per lesson are stored (Analytics Engine), never code. Yaniv sees what trips the class. | ≈$0.02 per 1,000 | M |
| **"Will this run forever?" pre-run nudge** | Jev Noul on edited code: "The program contains an unbounded loop without sleep or a blocking call." Warn about the CPU limit before Run. Advisory only. | ≈$0.05 per 1,000 | S |
| **Socratic follow-up** | After a hint, "Still stuck?" allows one more, more specific hint, still no full code. Cap at 3 per error. | Same as #2 | S |
| **Hebrew output** | Add `lang` to `/ai/hint`; the model answers in Hebrew. Code identifiers stay English. | Same | S |

---

## Recommendation (build order)

1. **Deterministic error hints** (§2 table). Zero cost, zero privacy concerns, covers most real student errors. Do this first regardless.
2. **Build-time "What to notice" notes** (§4). About ten cents total, teacher-reviewed, immediately improves every lesson page.
3. **`/ai/hint` route + "Explain this error" chip** (§2). This is the single runtime AI feature worth its complexity. Ship it with the cache, rate limit, daily budget and key spend cap from day one.
4. **Stuck-on-input chip** (§3). Nearly free once #3 exists.
5. **Jev command telemetry** (§1). Only after the OS-level hardening is live and validated, and only as async flag-and-notice. It improves visibility, not security.

Skip the inline Jev gate. It adds latency, can be evaded in one line, and false-positives on the course's own material.

## Open questions for Yaniv

1. Is sending student code to third-party model providers acceptable under Hadassah policy? Is opt-in per click plus a notice enough?
2. Hints only, or may it show a corrected snippet after N failed attempts?
3. Should hint answers be in Hebrew, English, or follow a site toggle?
4. What monthly AI budget cap should we set (e.g., $5)? This sets the OpenRouter key limit and the daily stop.
5. Should AI hints be disabled on exam-prep lessons or during exam periods?
6. Do you want the per-lesson common-mistake dashboard (§5)?
