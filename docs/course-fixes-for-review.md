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
