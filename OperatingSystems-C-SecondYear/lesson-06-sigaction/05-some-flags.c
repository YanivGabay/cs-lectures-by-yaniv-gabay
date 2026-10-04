/*
 * 05-some-flags.c — sigaction flags — SA_RESTART, SA_RESETHAND
 *
 * Key concepts: SA_RESTART (auto-retry interrupted syscalls), SA_RESETHAND (one-shot)
 * Compile: gcc -o flags 05-some-flags.c
 * Run:     ./prog
 */
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>


// here we show example of two flags
// SA_RESTART and SA_RESETHAND
// SA_RESTART is used to restart the system call if it was interrupted by the signal (sleep in this case)
// SA_RESETHAND is used to reset the handler to default after the first signal

// here we want to use :
// kill -SIGUSR1 <pid>
// than again
// kill -SIGUSR1 <pid>

// Handler for SIGUSR1
void handle_sigusr1(int signum) {
    printf("\n[Handler] Caught SIGUSR1 (signal %d).\n", signum);
    printf("[Handler] SA_RESETHAND: this handler is NOW reset to default (SIG_DFL).\n");
    printf("[Handler] SA_RESTART:   the sleep() that was interrupted will auto-restart.\n");
    printf("[Handler] Next SIGUSR1 will TERMINATE the process (default action).\n\n");
}

int main() {
    printf("\n");
    printf("══════════════════════════════════════\n");
    printf("  sigaction Flags Demo\n");
    printf("  SA_RESTART | SA_RESETHAND\n");
    printf("══════════════════════════════════════\n\n");
    struct sigaction sa;

    // Setup handler for SIGUSR1 with SA_RESTART and SA_RESETHAND flags
    sa.sa_handler = handle_sigusr1;
    sa.sa_flags = SA_RESTART | SA_RESETHAND; // Restart interrupted syscalls and reset handler
    sigemptyset(&sa.sa_mask);                  // No additional signals blocked

    if (sigaction(SIGUSR1, &sa, NULL) == -1) {
        perror("Error: sigaction for SIGUSR1 failed");
        exit(EXIT_FAILURE);
    }

    printf("[Main] sa_flags = SA_RESTART | SA_RESETHAND\n");
    printf("[Main]   SA_RESTART  — interrupted sleep() auto-restarts\n");
    printf("[Main]   SA_RESETHAND — handler reverts to SIG_DFL after first catch\n\n");
    printf("[Main] Process PID: %d\n", getpid());
    printf("[Main] 1st: kill -SIGUSR1 %d  → handler runs, then resets\n", getpid());
    printf("[Main] 2nd: kill -SIGUSR1 %d  → default action (terminate!)\n\n", getpid());

    // Infinite loop to keep the program running and handling signals
    int tick = 0;
    while (1) {
        printf("[Main] Tick %d — Waiting for SIGUSR1... (PID %d)\n", ++tick, getpid());
        sleep(4);
    }

    return 0;
}
