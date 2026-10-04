/*
 * 03-multi-signals.c — Handling multiple signals with sigaction
 *
 * Key concepts: multiple signal handlers, SIGINT, SIGTERM, SIGUSR1
 * Compile: gcc -o multi 03-multi-signals.c
 * Run:     ./prog
 */
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>


// in order to exit this program 
// ctrl + c wont work
// so we use the terminal to send a signal to the program
// kill -SIGTERM <pid>


// Handler for SIGINT
void handle_sigint(int signum);

// Handler for SIGTERM
void handle_sigterm(int signum);

int main() {
    printf("\n");
    printf("══════════════════════════════════════\n");
    printf("  Multiple Signal Handlers\n");
    printf("  SIGINT (Ctrl+C) vs SIGTERM (kill)\n");
    printf("══════════════════════════════════════\n\n");
    struct sigaction sa_int, sa_term;

    // Setup handler for SIGINT
    sa_int.sa_handler = handle_sigint;
    sa_int.sa_flags = 0;
    sigemptyset(&sa_int.sa_mask);

    if (sigaction(SIGINT, &sa_int, NULL) == -1) {
        perror("Error: sigaction for SIGINT failed");
        exit(EXIT_FAILURE);
    }
    printf("[Main] Registered handle_sigint  for SIGINT  (signal 2)\n");

    // Setup handler for SIGTERM
    sa_term.sa_handler = handle_sigterm;
    sa_term.sa_flags = 0;
    sigemptyset(&sa_term.sa_mask);

    if (sigaction(SIGTERM, &sa_term, NULL) == -1) {
        perror("Error: sigaction for SIGTERM failed");
        exit(EXIT_FAILURE);
    }
    printf("[Main] Registered handle_sigterm for SIGTERM (signal 15)\n\n");

    printf("[Main]  Process PID: %d\n", getpid());
    printf("[Main]  Ctrl+C sends SIGINT  — our handler catches it (won't exit)\n");
    printf("[Main]  To exit: run 'kill %d' from another terminal (sends SIGTERM)\n\n", getpid());

    // Infinite loop to keep the program running
    int tick = 0;
    while (1) {
        printf("[Main] Tick %d — Waiting for signals... (PID %d)\n", ++tick, getpid());
        sleep(3);
    }

    return 0;
}

// Functions

void handle_sigint(int signum) {
    printf("\n[Handler] Caught SIGINT (signal %d) — ignoring, still running!\n", signum);
    printf("[Handler] To actually exit, send SIGTERM: kill %d\n\n", getpid());
}

// Handler for SIGTERM
void handle_sigterm(int signum) {
    printf("\n[Handler] Caught SIGTERM (signal %d) — shutting down.\n", signum);
    exit(0);
}