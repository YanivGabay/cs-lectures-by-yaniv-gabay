/*
 * 02b-signals-from-terminal.c — Receive signals sent with the `kill` command from another terminal
 *
 * Key concepts: SIGUSR1/SIGUSR2, the kill command, pause(), one handler for several signals
 * Compile: gcc -o sigusr 02b-signals-from-terminal.c
 * Run:     ./sigusr      then, in a second terminal:  kill -USR1 <pid>   kill -USR2 <pid>   kill -TERM <pid>
 *          (Run pair does it for you in the right-hand terminal)
 *
 * SIGUSR1 and SIGUSR2 mean whatever your program decides. Here SIGTERM ends the loop politely.
 */
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

volatile sig_atomic_t last = 0;            // written in the handler, read in main

void on_signal(int sig)
{
    last = sig;                            // keep handlers tiny: just remember what came
}

int main(void)
{
    signal(SIGUSR1, on_signal);
    signal(SIGUSR2, on_signal);
    signal(SIGTERM, on_signal);

    printf("[PID %d] waiting for signals — try: kill -USR1 %d\n", getpid(), getpid());
    fflush(stdout);

    for (;;) {
        pause();                           // sleep until any signal arrives
        if (last == SIGUSR1)
            printf("  got SIGUSR1 — user signal 1\n");
        else if (last == SIGUSR2)
            printf("  got SIGUSR2 — user signal 2\n");
        else if (last == SIGTERM) {
            printf("  got SIGTERM — cleaning up and exiting\n");
            return 0;
        }
        fflush(stdout);
    }
}
