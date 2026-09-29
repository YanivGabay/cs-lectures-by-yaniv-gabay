/*
 * 01-basic_signals.c — Catching SIGINT (Ctrl+C) with signal()
 *
 * Demonstrates: Registering a signal handler with signal()
 * Key concepts: SIGINT, signal handlers, the program keeps running after catching
 * Compile: gcc -o signals 01-basic_signals.c
 * Run:     ./signals   (press Ctrl+C to trigger the handler)
 */

#include <sys/types.h>
#include <signal.h>    // for signal(), SIGINT
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>    // for sleep()

void catch_int(int sig_num);

int main() {

    printf("\n");
    printf("══════════════════════════════════════\n");
    printf("  Catching SIGINT (Ctrl+C) with signal()\n");
    printf("══════════════════════════════════════\n\n");
    // Register catch_int as the handler for SIGINT (Ctrl+C)
    // signal() returns SIG_ERR on failure
    signal(SIGINT, catch_int);

    printf("\n");
    printf("[Main PID %d] Running... Press Ctrl+C to trigger the signal handler.\n", getpid());
    printf("  (The handler catches SIGINT but doesn't exit — try pressing Ctrl+C multiple times!)\n");
    printf("  (To actually quit, press Ctrl+\\ which sends SIGQUIT)\n\n");

    int count = 0;
    while (1) {
        printf("[Main PID %d] Still running... (tick %d)\n", getpid(), ++count);
        sleep(2);
    }

    return 0;
}

void catch_int(int sig_num) {
   printf("\n[Signal Handler] Caught signal %d (SIGINT) — Ctrl+C pressed! But I'm not exiting.\n", sig_num);
    //exit(0); 
 
}