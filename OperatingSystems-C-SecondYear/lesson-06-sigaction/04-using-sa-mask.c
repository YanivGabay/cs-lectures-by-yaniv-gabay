/*
 * 04-using-sa-mask.c — Blocking signals during handler execution with sa_mask
 *
 * Key concepts: sa_mask, sigaddset(), signals blocked while handler runs
 * Compile: gcc -o mask 04-using-sa-mask.c
 * Run:     ./prog
 */
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>



// this program shows how the sa_mask works
// basicly its blocks the signal that is in the mask
// while the handler is running
// so in this program, we want first to send the sigusr1 signal
// and then the sigusr2 signal (we have 5 secs window to send the sigusr2 signal)
// but the sigusr2 signal will be blocked until the handler of sigusr1 is done

// Handler for SIGUSR1
void handle_sigusr1(int signum) {
    printf("\n[USR1 Handler] Entered! Signal %d received.\n", signum);
    printf("[USR1 Handler] SIGUSR2 is BLOCKED while this handler runs (sa_mask).\n");
    printf("[USR1 Handler] Sleeping 5 seconds... send SIGUSR2 now to test blocking!\n");
    fflush(stdout);
    // Simulate long processing time
    sleep(5);
    printf("[USR1 Handler] Done. SIGUSR2 will be delivered NOW if it was pending.\n\n");
}

// Handler for SIGUSR2
void handle_sigusr2(int signum) {
    printf("[USR2 Handler] Caught signal %d (SIGUSR2).\n\n", signum);
}

int main() {
    printf("\n");
    printf("══════════════════════════════════════\n");
    printf("  sa_mask — Blocking Signals in Handler\n");
    printf("  SIGUSR2 is blocked while SIGUSR1 handler runs\n");
    printf("══════════════════════════════════════\n\n");
    struct sigaction sa_usr1, sa_usr2;

    // Setup handler for SIGUSR1 with SIGUSR2 blocked during handler
    sa_usr1.sa_handler = handle_sigusr1;
    sa_usr1.sa_flags = 0;
    sigemptyset(&sa_usr1.sa_mask);          // Initialize sa_mask
    sigaddset(&sa_usr1.sa_mask, SIGUSR2);   // Block SIGUSR2 during SIGUSR1 handler

    if (sigaction(SIGUSR1, &sa_usr1, NULL) == -1) {
        perror("Error: sigaction for SIGUSR1 failed");
        exit(EXIT_FAILURE);
    }
    printf("[Main] SIGUSR1 handler registered (sa_mask includes SIGUSR2)\n");

    // Setup handler for SIGUSR2
    sa_usr2.sa_handler = handle_sigusr2;
    sa_usr2.sa_flags = 0;
    sigemptyset(&sa_usr2.sa_mask);          // No additional signals blocked

    if (sigaction(SIGUSR2, &sa_usr2, NULL) == -1) {
        perror("Error: sigaction for SIGUSR2 failed");
        exit(EXIT_FAILURE);
    }
    printf("[Main] SIGUSR2 handler registered (sa_mask empty)\n\n");

    printf("[Main] Process PID: %d\n", getpid());
    printf("[Main] Step 1: kill -SIGUSR1 %d   (starts 5s handler)\n", getpid());
    printf("[Main] Step 2: kill -SIGUSR2 %d   (within 5s — will be blocked!)\n", getpid());
    printf("[Main] Watch: SIGUSR2 handler runs AFTER SIGUSR1 handler finishes.\n\n");

    // Infinite loop to keep the program running
    int tick = 0;
    while (1) {
        printf("[Main] Tick %d — Waiting for signals... (PID %d)\n", ++tick, getpid());
        sleep(2);
    }

    return 0;
}
