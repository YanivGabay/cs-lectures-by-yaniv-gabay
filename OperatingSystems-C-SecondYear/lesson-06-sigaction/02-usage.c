/*
 * 02-usage.c — Basic sigaction usage — registering a handler
 *
 * Key concepts: sigaction(), sigemptyset(), signal registration
 * Compile: gcc -o usage 02-usage.c
 * Run:     ./prog
 */
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

// Handler function for SIGINT
void sigint_handler(int signum) {
    printf("\n[Handler] Caught signal %d (SIGINT). Exiting gracefully.\n", signum);
    exit(0);
}

int main() {
    printf("\n");
    printf("══════════════════════════════════════\n");
    printf("  Basic sigaction() Usage\n");
    printf("  Register a custom handler for SIGINT\n");
    printf("══════════════════════════════════════\n\n");
    struct sigaction sa;

    // Set up the sigaction struct
    sa.sa_handler = sigint_handler;    // Assign handler function
    sa.sa_flags = 0;                    // No special flags
    sigemptyset(&sa.sa_mask);           // Don't block any signals during handler

    // Set up the handler for SIGINT
    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("Error: sigaction failed");
        exit(EXIT_FAILURE);
    }

    printf("[Main] sigaction() registered handler for SIGINT\n");
    printf("[Main] sa_handler = sigint_handler, sa_flags = 0\n");
    printf("[Main] sa_mask = empty (no signals blocked during handler)\n");
    printf("[Main] Process PID: %d\n\n", getpid());
    printf("Press Ctrl+C to trigger SIGINT...\n\n");

    // Infinite loop to keep the program running
    int tick = 0;
    while (1) {
        printf("[Main] Tick %d — Running... (Ctrl+C to exit)\n", ++tick);
        sleep(2);
    }

    return 0;
}
