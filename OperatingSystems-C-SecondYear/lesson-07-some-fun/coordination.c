/*
 * coordination.c — Parent-child signal coordination — synchronized printing
 *
 * Key concepts: kill(), SIGUSR1, parent and child take turns via signals
 * Compile: gcc -o coord coordination.c
 * Run:     ./prog
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>

// Global variables to store child PIDs
pid_t child1_pid = 0;
pid_t child2_pid = 0;
volatile sig_atomic_t all_done = 0; // set by the SIGUSR2 handler so main can stop waiting

// Function to set up signal handlers using sigaction
void setup_sigaction(int signum, void (*handler)(int)) {
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = handler;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);

    if (sigaction(signum, &sa, NULL) == -1) {
        perror("Failed to set up sigaction");
        exit(EXIT_FAILURE);
    }
}

// Signal handler for SIGUSR1 (from Child 1)
void handle_sigusr1(int signum) {
    printf("\n[Parent] Received SIGUSR1 from Child 1 — task acknowledged.\n");
    printf("[Parent] Sending SIGUSR2 to Child 2 (PID %d) to start its task...\n", child2_pid);
    if (kill(child2_pid, SIGUSR2) == -1) {
        perror("[Parent] Failed to send SIGUSR2 to Child 2");
    }
}

// Signal handler for SIGUSR2 (from Child 2)
void handle_sigusr2(int signum) {
    printf("\n[Parent] Received SIGUSR2 from Child 2 — all tasks completed!\n");
    printf("[Parent] Terminating both children...\n");
    kill(child1_pid, SIGTERM);
    kill(child2_pid, SIGTERM);
    all_done = 1;
}

// Child 2 needs a handler (even an empty one): with SIG_DFL, SIGUSR2 would kill it instead of waking pause()
void child_wakeup(int signum) {
    (void)signum;
}

int main() {
    printf("\n");
    printf("══════════════════════════════════════\n");
    printf("  Parent-Child Signal Coordination\n");
    printf("  Child1 → Parent → Child2 → Parent\n");
    printf("══════════════════════════════════════\n\n");
    // Set up the signal handlers in the parent using sigaction
    setup_sigaction(SIGUSR1, handle_sigusr1);
    setup_sigaction(SIGUSR2, handle_sigusr2);

    printf("[Parent PID %d] Signal handlers registered.\n\n", getpid());

    // Fork Child 1
    child1_pid = fork();
    if (child1_pid < 0) {
        perror("Failed to fork Child 1");
        exit(EXIT_FAILURE);
    }

    if (child1_pid == 0) {
        // In Child 1
        printf("[Child 1 PID %d] Started. Working for 2 seconds...\n", getpid());
        sleep(2);
        printf("[Child 1 PID %d] Done! Sending SIGUSR1 to Parent (PID %d).\n", getpid(), getppid());
        kill(getppid(), SIGUSR1);
        pause();
        exit(0);
    }

    // Fork Child 2
    child2_pid = fork();
    if (child2_pid < 0) {
        perror("Failed to fork Child 2");
        kill(child1_pid, SIGTERM);
        exit(EXIT_FAILURE);
    }

    if (child2_pid == 0) {
        // In Child 2
        printf("[Child 2 PID %d] Started. Waiting for signal from Parent...\n", getpid());
        setup_sigaction(SIGUSR2, child_wakeup);
        pause();
        printf("[Child 2 PID %d] Received signal! Task done. Sending SIGUSR2 to Parent.\n", getpid());
        kill(getppid(), SIGUSR2);
        exit(0);
    }

    printf("[Parent] Forked Child 1 (PID %d) and Child 2 (PID %d).\n", child1_pid, child2_pid);
    printf("[Parent] Waiting for Child 1 to finish its work...\n\n");
    while (!all_done) {
        pause(); // Wait for signals until Child 2 reports back
    }

    //wait on all the children
    while (wait(NULL) > 0);

    return 0;
}
