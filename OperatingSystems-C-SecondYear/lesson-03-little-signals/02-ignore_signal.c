/*
 * 02-ignore_signal.c — Ignoring signals with SIG_IGN
 *
 * Demonstrates: Using SIG_IGN to make a process immune to SIGINT
 * Key concepts: SIG_IGN, Ctrl+C won't stop this program (use kill -9 or close terminal)
 * Compile: gcc -o ignore 02-ignore_signal.c
 * Run:     ./ignore   (Ctrl+C will NOT work — use Ctrl+\ for SIGQUIT or kill the process)
 */

#include <sys/types.h>
#include <signal.h>    // for signal(), SIG_IGN, SIGINT
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>    // for sleep()

void catch_int(int sig_num);

int main() {
    // SIG_IGN = "ignore this signal" — Ctrl+C will be silently discarded
    signal(SIGINT, SIG_IGN);

    printf("\n");
    printf("========================================\n");
    printf("  Ignoring Signals with SIG_IGN\n");
    printf("========================================\n\n");
    printf("[PID %d] SIGINT (Ctrl+C) is now IGNORED.\n", getpid());
    printf("  Try pressing Ctrl+C — nothing will happen!\n");
    printf("  To quit: press Ctrl+\\ (sends SIGQUIT, which is NOT ignored)\n\n");

    int count = 0;
    while (1) {
        printf("[PID %d] Still running... (tick %d)\n", getpid(), ++count);
        sleep(2);
    }

    return 0;
}

void catch_int(int sig_num) {
   printf("Caught signal %d\n", sig_num);
    exit(0); 
 
}