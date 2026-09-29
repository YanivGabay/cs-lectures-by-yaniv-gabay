/*
 * multiply_signals.c — Multiplexing signals across 4 child processes
 *
 * Key concepts: Multiple children, signal routing, wait, kill
 * Compile: gcc -o multi_sig multiply_signals.c
 * Run:     ./prog
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>

// Number of child processes
#define NUM_CHILDREN 4

// Function prototypes for signal handlers
void handle_sigint(int sig);
void handle_sigusr1(int sig);
void handle_sigusr2(int sig);
void handle_sigterm(int sig);

int main() {
    printf("\n");
    printf("══════════════════════════════════════\n");
    printf("  Signal Multiplexing — 4 Children\n");
    printf("  Each child sends a different signal\n");
    printf("══════════════════════════════════════\n\n");
    pid_t pid;
    struct sigaction sa_int, sa_usr1, sa_usr2, sa_term;

    // Setting up SIGINT handler
    sa_int.sa_handler = handle_sigint;
    sa_int.sa_flags = 0;
    sigemptyset(&sa_int.sa_mask);
    if (sigaction(SIGINT, &sa_int, NULL) == -1) {
        perror("Error: cannot handle SIGINT");
        exit(EXIT_FAILURE);
    }

    // Setting up SIGUSR1 handler
    sa_usr1.sa_handler = handle_sigusr1;
    sa_usr1.sa_flags = 0;
    sigemptyset(&sa_usr1.sa_mask);
    if (sigaction(SIGUSR1, &sa_usr1, NULL) == -1) {
        perror("Error: cannot handle SIGUSR1");
        exit(EXIT_FAILURE);
    }

    // Setting up SIGUSR2 handler
    sa_usr2.sa_handler = handle_sigusr2;
    sa_usr2.sa_flags = 0;
    sigemptyset(&sa_usr2.sa_mask);
    if (sigaction(SIGUSR2, &sa_usr2, NULL) == -1) {
        perror("Error: cannot handle SIGUSR2");
        exit(EXIT_FAILURE);
    }

    // Setting up SIGTERM handler
    sa_term.sa_handler = handle_sigterm;
    sa_term.sa_flags = 0;
    sigemptyset(&sa_term.sa_mask);
    if (sigaction(SIGTERM, &sa_term, NULL) == -1) {
        perror("Error: cannot handle SIGTERM");
        exit(EXIT_FAILURE);
    }

    printf("[Parent PID %d] Handlers registered for SIGINT, SIGUSR1, SIGUSR2, SIGTERM.\n\n", getpid());

    // Array of signals to be sent by children
    const char *sig_names[NUM_CHILDREN] = {"SIGINT", "SIGUSR1", "SIGUSR2", "SIGTERM"};
    int signals[NUM_CHILDREN] = {SIGINT, SIGUSR1, SIGUSR2, SIGTERM};

    // Forking child processes
    for (int i = 0; i < NUM_CHILDREN; i++) {
        pid = fork();
        if (pid < 0) {
            perror("Fork failed");
            exit(EXIT_FAILURE);
        }

        if (pid == 0) {
            // Child process
            int delay = 2 * (i + 1);
            printf("[Child %d PID %d] Will send %s in %d seconds.\n", i, getpid(), sig_names[i], delay);
            fflush(stdout);
            sleep(delay);
            printf("[Child %d PID %d] Sending %s to Parent (PID %d)...\n", i, getpid(), sig_names[i], getppid());
            if (kill(getppid(), signals[i]) == -1) {
                perror("Failed to send signal");
                exit(EXIT_FAILURE);
            }
            exit(EXIT_SUCCESS);
        }
    }

    // Parent process waits for signals indefinitely
    printf("\n[Parent] All children forked. Waiting for signals...\n");
    printf("[Parent] Signals will arrive staggered: 2s, 4s, 6s, 8s.\n\n");

    while (1) {
        pause(); // Wait for signals
    }

    //

    // This point is never reached
    return 0;
}

// Handler for SIGINT
void handle_sigint(int sig) {
    printf("\n[Handler] SIGINT (signal %d) — Interrupt. Handling gracefully.\n\n", sig);
}

void handle_sigusr1(int sig) {
    printf("[Handler] SIGUSR1 (signal %d) — User-defined signal 1 received.\n\n", sig);
}

void handle_sigusr2(int sig) {
    printf("[Handler] SIGUSR2 (signal %d) — User-defined signal 2 received.\n\n", sig);
}

void handle_sigterm(int sig) {
    printf("[Handler] SIGTERM (signal %d) — Termination. Cleaning up...\n", sig);
    // Perform any necessary cleanup here
    

    //before exiting, we need to wait for all the children to finish
    //we can do that by using the wait function

    while (wait(NULL) > 0); // Wait for all child processes to finish
    exit(EXIT_SUCCESS);
}
