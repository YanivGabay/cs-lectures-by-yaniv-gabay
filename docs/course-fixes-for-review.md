# Course code changes — for Yaniv's review

The example audit (`validate/examples.mjs`) runs every lesson example in the real sandbox
exactly like the Run button. These files did not compile with today's gcc (15, musl) or did
not do what their comments say. Each change is minimal; `git diff` shows the exact lines.

| File | Problem found | Change |
|---|---|---|
| `lesson-07-some-fun/coordination.c` | Child 2 set SIGUSR2 to `SIG_DFL`, so the signal meant to wake it **killed** it; it never reported back and the parent `pause()`d forever. The final `wait()` loop was unreachable. | Child 2 installs an empty handler (`child_wakeup`) so `pause()` returns; the SIGUSR2 handler sets `all_done`, and the parent loops `while (!all_done) pause();`. Now runs to completion. |
| `lesson-08-pipes/02-pipe-operator-with-programs/producer.c` | The "═══ Producer ═══" banner (added by the output-polish commit `c1cab23`) went to **stdout — the pipe** — so the consumer read the banner instead of "Yaniv". | Banner printed to stderr; only the data goes through the pipe. |
| `lesson-08-pipes/02-pipe-operator-with-programs/consumer.c` | `scanf("%s")` read the 114-byte banner into `char buffer[100]` → **segfault**. | `scanf("%99s", buffer)` — a bounded read. |
| `lesson-08-pipes/03.5-pipe-reading-1-byte.c` | `wait()` used without `<sys/wait.h>` (an error since gcc 14). | Added the include. |
| `lesson-09-named-pipe-msg-que/04-mk-fifo-two-way/process_b.c` | `unlink(FIFO_NAME)` — `FIFO_NAME` does not exist in this file. | `unlink(FIFO_A_TO_B); unlink(FIFO_B_TO_A);` |
| `lesson-10-shared-memory/03-positions/position_viewer.c` | `#include "position.h"` — the header is not in the repository. | Inlined the same `Position` typedef the creator and updater use. |
| `lesson-10-shared-memory/04-array/{creator,producer,consumer}.c` | `const char* SHM_KEY = '0x2345';` — a multi-character **char** constant stored in a pointer, then passed to `shmget()` as the key. | `const key_t SHM_KEY = 0x2345;` (+ `<signal.h>` in creator.c, which calls `signal()`). |
| `lesson-09-named-pipe-msg-que/08-calc-que/calc_receiver.c` | The server read "any" message type, so it sometimes read back its **own reply** (type 2) before the client did — both then waited forever (about 1 run in 4). | `msgrcv(..., MSG_TYPE_CALC_RESPONSE, MSG_EXCEPT)` + `#define _GNU_SOURCE`: take everything except replies. |
| `lesson-12-pthreads/10-async-chat.c` | `strncpy` without `<string.h>`. | Added the include. |

## Not changed in the source (handled in the site's example data instead)

- **`lesson-01-intro-to-c/03-more-basic-input-output.c`** uses `gets()`, which C11 removed. Compiled
  with `-std=gnu99` so the demo of the unsafe call still builds (with a warning).
- **`lesson-12-pthreads/03-pthread_cleanup.c`** returns from inside `pthread_cleanup_push/pop` one
  time in ten. That is undefined behaviour; glibc tolerates it, musl crashes (SIGSEGV). The comments
  already discuss it, so the site labels the crash as intentional rather than changing the lesson.
- **`lesson-08-pipes/08-two-ways-with-dupes/parent-program.c`** execs `./child_program`, but the old
  compile line built `child_dup`. The Run command now builds the child under the name the parent expects.
- **`lesson-07-some-fun/alarm_manager/`**: `gcc -o alarm_manager` collided with the folder of the
  same name; the output is now `alarm_mgr`, and `alarm_handler` is built first because the manager execs it.

## Classroom note (not a code bug)

Lessons 09–11 use fixed System V keys (`0x1234`, `ftok("/tmp", 65)`) and fixed ports (3879…).
On a shared machine two students would collide. The sandbox now gives each terminal session its
own IPC, network and `/tmp` namespaces (see Lesson 16), so the examples work unchanged.

## New in Lesson 01.5 (prepared for the next lecture — please review)

New files in `lesson-01.5-more-intro-to-c/`, each one idea, in teaching order on the lesson page:

| File | Teaches | Run on the site |
|---|---|---|
| `args-sum.c` | arguments are strings; `atoi` vs `strtol` with validation | `./sum 5 7 30; ./sum 5 seven 3` |
| `args-flags.c` | options like `-n 3 -u`, `strcmp`, usage message, exit status 1 | `./greet -n 3 -u Dana; ./greet -n` |
| `args-wc.c` | mini `wc` over several files, `perror` and keep going | `./mini_wc poem.txt notes.txt missing.txt` |
| `args-copy.c` | your own `cp`: two files open, `"r"`/`"w"`, check every call | `./mycp notes.txt copy.txt` |
| `poem.txt` | input for `args-wc.c` (editable in the Files panel) | — |

Existing files are unchanged; `argc_argv.c` now runs with arguments (`./argc_argv hello world 42 "two words"`).
New challenge "Check argc first" (`site/src/data/challenges/args-check.c`): the starter crashes with
SIGSEGV when run without arguments; the fix checks `argc`, prints a usage line and exits 1.

## New in Lesson 02 (prepared for the lecture after 01.5 — please review)

New files in `lesson-02-basic-forks/`, one idea each, placed in teaching order on the lesson page:

| File | Teaches | Run on the site |
|---|---|---|
| `01a-fork-copy.c` | the child gets a *copy* of memory: same `&x`, different values | `./fork_copy` |
| `01b-fork-buffer.c` | `fork()` copies the unflushed stdout buffer; to a file even whole lines double (`out.txt` in the Files panel); fix: `fflush` | `./fork_buffer; echo; ./fork_buffer > out.txt` |
| `03a-wait-status.c` | `waitpid` + `WIFEXITED`/`WEXITSTATUS`/`WIFSIGNALED`/`WTERMSIG` for exit 0, exit 42, SIGSEGV | `./wait_status` |
| `03b-zombie-live.c` | a real zombie: `ps` shows `Z <defunct>` until `wait()` | `./zombie` |
| `03c-orphan.c` | parent exits first; child reports adoption by PID 1 (its last line comes after the prompt) | `./orphan` |

Existing files are unchanged. The reference text `01.5-forking-zombies.c` now sits right after the live zombie.
Note on `01b-fork-buffer.c`: in this sandbox (musl) the very first line is flushed before `fork()` even when
redirected; the comment explains it, and on a glibc laptop that line doubles too.

## New in Lesson 03 (prepared for the lecture after 02 — please review)

New files in `lesson-03-little-signals/`, one idea each, placed in teaching order:

| File | Teaches | On the site |
|---|---|---|
| `01a-default-action.c` | no handler → the default action terminates (`raise(SIGUSR1)`) | exit chip "Killed by SIGUSR1 (10)" |
| `02a-kill-child.c` | `kill()` from parent to child; `waitpid` shows signal 15 | `./kill_child` |
| `02b-signals-from-terminal.c` | SIGUSR1/SIGUSR2/SIGTERM from the `kill` command; `pause()` | Run pair: the right terminal sends the three signals |
| `04a-graceful-shutdown.c` | handler only sets a `volatile sig_atomic_t` flag; main finishes and closes the file | auto Ctrl+C at 2.5 s; `work.log` in the Files panel |

Existing files are unchanged. New on the site: the existing Ctrl+C demos no longer wait for someone to press
Ctrl+C — the site presses it once after 2.5 s (`interruptAfterMs`): `01-basic_signals` (handler runs, keeps
going), `02-ignore_signal` (nothing happens), `04-sig_init_once` (first handled, a second Ctrl+C at 6 s kills it).

## Lesson 04 (prepared for the lecture after 03 — please review)

**Fixed:** `calculator_exec.c` failed on the site with "exec failed: No such file or directory" — the
child execs `./calculator`, which was never built (only the launcher was uploaded). Run now builds
`calculator` first. The audit used to call it a pass because the parent still exits 0; it now flags
any "exec … failed: No such file or directory" (`exec-failed`).

New files in `lesson-04-exec/`, one idea each, in teaching order:

| File | Teaches | On the site |
|---|---|---|
| `01-exec-replaces.c` | same PID before and after exec; the line after exec never runs | `./exec_replaces` |
| `02-exec-fails.c` | exec returns -1 only on failure: ENOENT vs EACCES, exit 127 | `./exec_fails` |
| `03-exec-env.c` | `execle` with a chosen environment; `env` shows only those variables | `./exec_env` |
| `04-exec-redirect.c` | open files survive exec: `dup2` + `execlp("ls")` → `listing.txt` | Files panel shows `listing.txt` |
| `05-mini-shell.c` | a tiny shell: `fgets` → `strtok` argv → fork + `execvp` + `waitpid` | sample input: echo, ls, wc, a missing command, exit |

Existing files are unchanged.

## Lesson 05 (prepared for the lecture after 04 — please review)

The two central examples (`main.c`, `weird_behaviours/shell.c`) use Windows `process.h`, so on the site they
stay read-only. New runnable Linux counterparts in `lesson-05-spawn-windows/`, each placed right after the
Windows file it mirrors:

| File | Mirrors | On the site |
|---|---|---|
| `01-posix-spawn.c` | `_spawnl` / `_spawnlp` / `_P_WAIT` / `_P_NOWAIT` in `main.c` | `posix_spawn` + waitpid now or later; `posix_spawnp` searches PATH |
| `show_args.c` | — (helper) | prints each argument in brackets: `./show_args one "two words" "*"` |
| `02-args-shell-vs-spawn.c` | the spaces and `*` quirks in `shell.c` / `main.c` | spawn keeps `{"aa bb cc", "*"}` (2 args); `system()` → `/bin/sh` splits and expands `*` into file names; quoting fixes it |
| `03-spawn-redirect.c` | — | `posix_spawn_file_actions_addopen` → calculator output in `result.txt` (Files panel) |

Lesson title on the site: "Process Spawning: Windows and Linux" (slug unchanged). Existing files are unchanged;
the Windows files' notes now point to their Linux counterparts.
