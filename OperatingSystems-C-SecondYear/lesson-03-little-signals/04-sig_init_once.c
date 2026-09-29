/*
 * 04-sig_init_once.c — One-shot signal handler (catch once, then default)
 *
 * Demonstrates: Restoring default signal behavior with SIG_DFL inside a handler
 * Key concepts: First Ctrl+C is caught, second Ctrl+C kills the process
 * Compile: gcc -o once 04-sig_init_once.c
 * Run:     ./once   (press Ctrl+C twice)
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>    // for getpid(), sleep()
#include <signal.h>    // for signal(), SIGINT, SIG_DFL

void custom_handler(int sig) {
    printf("\n[Handler] Caught signal %d (SIGINT) — first Ctrl+C!\n", sig);
    printf("[Handler] Restoring default behavior with signal(SIGINT, SIG_DFL).\n");
    printf("[Handler] Next Ctrl+C will KILL the process.\n\n");
    signal(SIGINT, SIG_DFL);
}

int main() {

    printf("\n");
    printf("══════════════════════════════════════\n");
    printf("  One-shot signal handler (catch once, then default)\n");
    printf("══════════════════════════════════════\n\n");
    signal(SIGINT, custom_handler);

    printf("\n");
    printf("[Main PID %d] Custom SIGINT handler installed.\n", getpid());
    printf("  1st Ctrl+C → handler catches it, restores default\n");
    printf("  2nd Ctrl+C → default action (terminate process)\n\n");

    int count = 0;
    while (1) {
        printf("[Main PID %d] Running... (tick %d)\n", getpid(), ++count);
        sleep(2);
    }

    return 0;
}
