# Example readiness — 2026-10-06 21:23 UTC

Terminal: `wss://cs-lectures-terminal.yaniv242.workers.dev` · run limit 8s · 118 examples

| Category | Count |
|---|---|
| crashes | 1 |
| hangs | 1 |
| long-running | 15 |
| pair-partner | 12 |
| pair-runs | 12 |
| reference-only | 6 |
| runs-and-exits | 59 |
| runs-with-input | 10 |
| windows-only | 2 |

Unexplained problems: **1**

## 01 — Introduction to C

| File | Category | Detail | Data |
|---|---|---|---|
| `01-basic-input-output.c` | runs-with-input | status 0, fed sampleInput | interactive |
| `02-more-basic-input-output.c` | runs-with-input | status 0, fed sampleInput | interactive |
| `03-more-basic-input-output.c` | runs-with-input | status 0, fed sampleInput | interactive, intentional |
| `04-basic-files-ptrs.c` | runs-and-exits | status 0 |  |
| `05-more-files-struct.c` | runs-and-exits | status 0 |  |
| `06-complex-usage-scanf-printf.c` | runs-with-input | status 0, fed sampleInput | interactive |
| `07-another-complex.c` | runs-with-input | status 0, fed sampleInput | interactive |
| `08-dont-forget-functions.c` | runs-with-input | status 0, fed sampleInput | interactive |
| `09-last-example.c` | runs-and-exits | status 0 |  |
| `10-last-last-example.c` | runs-and-exits | status 0 |  |

## 01.5 — Command-Line Arguments

| File | Category | Detail | Data |
|---|---|---|---|
| `argc_argv_another_example.c` | runs-and-exits | status 0 |  |
| `argc_argv.c` | runs-and-exits | status 0 |  |

## 02 — Process Creation with fork()

| File | Category | Detail | Data |
|---|---|---|---|
| `00-some-includes.c` | reference-only | marked in data (not run) | reference |
| `01-basic-forking.c` | runs-and-exits | status 0 |  |
| `01.5-forking-zombies.c` | reference-only | marked in data (not run) | reference |
| `02-basic-fork-mistake.c` | runs-and-exits | status 0 |  |
| `03-basic-fork-thefix.c` | runs-and-exits | status 0 |  |
| `04-forks-gettimeday.c` | runs-and-exits | status 0 |  |

## 03 — Introduction to Signals

| File | Category | Detail | Data |
|---|---|---|---|
| `01-basic_signals.c` | long-running | stopped after 8000ms as configured | long-running, intentional |
| `02-ignore_signal.c` | long-running | stopped after 8000ms as configured | long-running, intentional |
| `03-sig_alarm.c` | runs-and-exits | status 0 |  |
| `04-sig_init_once.c` | long-running | stopped after 8000ms as configured | long-running, intentional |
| `05-sighandler_behaviour.c` | runs-with-input | status 0, fed sampleInput | interactive, intentional |
| `06-sigaction_behaviour.c` | runs-with-input | status 0, fed sampleInput | interactive, intentional |

## 04 — Program Execution with exec()

| File | Category | Detail | Data |
|---|---|---|---|
| `basic_exec.c` | runs-and-exits | status 0 |  |
| `binary_search_dupes.c` | runs-and-exits | status 0 |  |
| `calculator_exec.c` | runs-and-exits | status 0 |  |
| `calculator.c` | runs-and-exits | status 0 |  |

## 05 — Windows Process Spawning

| File | Category | Detail | Data |
|---|---|---|---|
| `calculator.c` | runs-and-exits | status 0 |  |
| `main.c` | windows-only | marked in data (not run) | windows |
| `weird_behaviours/shell.c` | windows-only | marked in data (not run) | windows |
| `weird_behaviours/unique_str.c` | runs-and-exits | status 0 |  |

## 06 — Advanced Signals with sigaction

| File | Category | Detail | Data |
|---|---|---|---|
| `01-basic-info.c` | reference-only | marked in data (not run) | reference |
| `02-usage.c` | long-running | stopped after 8000ms as configured | long-running |
| `03-multi-signals.c` | runs-and-exits | status 0; companion status 0 | long-running, intentional |
| `04-using-sa-mask.c` | long-running | stopped after 8000ms as configured | long-running |
| `05-some-flags.c` | crashes | status 138; companion status 0 | long-running, intentional |
| `06-sig-info.c` | long-running | stopped after 10000ms as configured; companion status 0 | long-running |
| `07-before-ex2.c` | long-running | stopped after 30000ms as configured | interactive |

## 07 — Signal Coordination

| File | Category | Detail | Data |
|---|---|---|---|
| `alarm_manager/alarm_handler.c` | reference-only | marked in data (not run) | reference |
| `alarm_manager/alarm_manager.c` | runs-with-input | status 0, fed sampleInput | interactive |
| `coordination.c` | runs-and-exits | status 0 |  |
| `multiply_signals.c` | hangs | no output for 1.8s while blocked |  |

## 08 — Pipes

| File | Category | Detail | Data |
|---|---|---|---|
| `01-basics.c` | reference-only | marked in data (not run) | reference |
| `02-pipe-operator-with-programs/consumer.c` | runs-and-exits | status 0 |  |
| `02-pipe-operator-with-programs/producer.c` | runs-and-exits | status 0 |  |
| `03-pipe-system-call.c` | runs-and-exits | status 0 |  |
| `03.5-pipe-reading-1-byte.c` | runs-and-exits | status 0 |  |
| `04-more-pipe.c` | runs-and-exits | status 0 |  |
| `05-pipe-with-fd.c` | runs-and-exits | status 0 |  |
| `06-pipe-exec-dup.c` | runs-and-exits | status 0 |  |
| `07-two-ways-communication.c` | runs-and-exits | status 0 |  |
| `08-two-ways-with-dupes/child.c` | reference-only | marked in data (not run) | reference |
| `08-two-ways-with-dupes/parent-program.c` | runs-and-exits | status 0 |  |
| `09-mkfifo/reader.c` | pair-runs | status 0/0 | pair |
| `09-mkfifo/writer.c` | pair-partner | runs with 09-mkfifo/reader.c (Run pair) |  |

## 09 — Named Pipes & Message Queues

| File | Category | Detail | Data |
|---|---|---|---|
| `02-mkfifo-example/fifo-reader.c` | pair-runs | status 0/0 | pair |
| `02-mkfifo-example/fifo-writer.c` | pair-partner | runs with 02-mkfifo-example/fifo-reader.c (Run pair) | pair |
| `03-mk-fifo-multi-writers/multi-writer.c` | pair-partner | runs with 03-mk-fifo-multi-writers/reader.c (Run pair) | pair |
| `03-mk-fifo-multi-writers/reader.c` | pair-runs | status 0/0 | pair |
| `04-mk-fifo-two-way/process_a.c` | pair-partner | runs with 04-mk-fifo-two-way/process_b.c (Run pair) | pair |
| `04-mk-fifo-two-way/process_b.c` | pair-runs | status 0/0 | pair |
| `05-msg-que-basic/02_aba_yeled.c` | runs-and-exits | status 0 |  |
| `05-msg-que-basic/03_bidirection.c` | runs-and-exits | status 0 |  |
| `05-msg-que-basic/aba_yeled.c` | runs-and-exits | status 0 |  |
| `06-ipcm-ipcrm-removing.c` | runs-and-exits | status 0 |  |
| `07-msg-que-two-progs/receiver.c` | pair-runs | status 0/0 | pair |
| `07-msg-que-two-progs/sender.c` | pair-partner | runs with 07-msg-que-two-progs/receiver.c (Run pair) | pair |
| `08-calc-que/calc_receiver.c` | pair-runs | status 0/0 | pair |
| `08-calc-que/calc_sender.c` | pair-partner | runs with 08-calc-que/calc_receiver.c (Run pair) | pair |

## 10 — Shared Memory

| File | Category | Detail | Data |
|---|---|---|---|
| `02-basic-example/consumer.c` | pair-partner | runs with 02-basic-example/creator.c (Run pair) | pair |
| `02-basic-example/creator.c` | pair-runs | status 0/0, fed sampleInput | pair |
| `03-positions/position_creator.c` | pair-runs | statuses stopped/stopped (stopped on schedule) | pair |
| `03-positions/position_updater.c` | pair-partner | runs with 03-positions/position_creator.c (Run pair) | pair |
| `03-positions/position_viewer.c` | runs-and-exits | status 1 | long-running |
| `04-array/consumer.c` | runs-and-exits | status 1 | long-running |
| `04-array/creator.c` | pair-runs | statuses stopped/stopped (stopped on schedule) | pair |
| `04-array/producer.c` | pair-partner | runs with 04-array/creator.c (Run pair) | pair |
| `05-scoreboard-struct/scoreboard_creator.c` | runs-and-exits | status 0 |  |
| `05-scoreboard-struct/scoreboard_updater.c` | runs-and-exits | status 1 |  |
| `05-scoreboard-struct/scoreboard_viewer.c` | long-running | stopped after 12000ms as configured; companion status null | long-running |

## 11 — TCP Sockets

| File | Category | Detail | Data |
|---|---|---|---|
| `01-example/echo-client.c` | pair-partner | runs with 01-example/echo-server.c (Run pair) | pair |
| `01-example/echo-server.c` | pair-runs | statuses stopped/0 (stopped on schedule) | pair |
| `02-chat/chat-client.c` | pair-partner | runs with 02-chat/chat-server.c (Run pair) | pair |
| `02-chat/chat-server.c` | pair-runs | statuses stopped/0 (stopped on schedule) | pair |
| `03-arithmetic/client.c` | pair-partner | runs with 03-arithmetic/server.c (Run pair) | pair |
| `03-arithmetic/server.c` | pair-runs | statuses stopped/0 (stopped on schedule) | pair |

## 12 — POSIX Threads

| File | Category | Detail | Data |
|---|---|---|---|
| `01-pthread_create.c` | runs-and-exits | status 0 |  |
| `02-pthread_exit.c` | runs-and-exits | status 0 |  |
| `03-pthread_cleanup.c` | runs-and-exits | status 0 | intentional |
| `04-pthread_join.c` | long-running | still printing when stopped |  |
| `05-pthread_ping_pong.c` | runs-and-exits | status 0 |  |
| `06-pthread-once.c` | runs-and-exits | status 0 |  |
| `07-partial-sum.c` | runs-and-exits | status 0 |  |
| `08-pthread-exit.c` | runs-and-exits | status 0 | long-running |
| `09-countdown-race.c` | long-running | still printing when stopped |  |
| `10-async-chat.c` | runs-and-exits | status 0 | long-running |
| `11-pthreads-fifo.c` | runs-and-exits | status 0 |  |

## 13 — Synchronization

| File | Category | Detail | Data |
|---|---|---|---|
| `07-named-semaphore.c` | runs-and-exits | status 0 | long-running |
| `08-pthread-mutex.c` | runs-and-exits | status 0 |  |
| `09-pthread-mutex-cond-wait.c` | long-running | stopped after 8000ms as configured | long-running |

## 14 — Advanced Threads & Concurrency

| File | Category | Detail | Data |
|---|---|---|---|
| `01-cli.c` | runs-and-exits | status 0 |  |
| `02-bit-more-cli/cli.c` | runs-with-input | status 0, fed sampleInput | interactive |
| `03-forks-mutex.c` | runs-and-exits | status 0 |  |
| `04-forks-mutex.c` | runs-and-exits | status 0 |  |
| `05-forks-philosph.c` | runs-and-exits | status 0 |  |
| `06-pthreads.c` | runs-and-exits | status 0 |  |
| `07-pthreads-2.c` | runs-and-exits | status 0 |  |
| `08-cool-bank.c` | runs-and-exits | status 0 |  |
| `09-cool-canvas.c` | long-running | stopped after 8000ms as configured | long-running |
| `10-spinners.c` | long-running | stopped after 8000ms as configured | long-running |
| `11-5-cond-waiting-bad.c` | long-running | stopped after 6000ms as configured | long-running, intentional |
| `12-cond-simple.c` | runs-and-exits | status 0 |  |
| `13-broadcast.c` | runs-and-exits | status 0 |  |

## 16 — How This Site Runs Your Code

| File | Category | Detail | Data |
|---|---|---|---|
| `ctrl-c-path.c` | long-running | stopped after 4000ms as configured | long-running |
| `fork-until-eagain.c` | runs-and-exits | status 0 |  |
| `sandbox-launch.c` | runs-and-exits | status 1 | intentional |
| `whoami-sandbox.c` | runs-and-exits | status 0 |  |
