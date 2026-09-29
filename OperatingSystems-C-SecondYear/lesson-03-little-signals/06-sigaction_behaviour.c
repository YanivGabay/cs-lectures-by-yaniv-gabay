/*
 * 06-sigaction_behaviour.c — sigaction vs signal — comparing behavior during I/O
 *
 * Key concepts: sigaction struct, SA_RESTART flag, reliable signal handling
 * Compile: gcc -o sigact 06-sigaction_behaviour.c
 * Run:     ./sigact
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>    // for getpid()
#include <signal.h>    // for sigaction(), SIGINT

void sigint_handler(int sig) {
    printf("\n[Handler] Caught signal %d (SIGINT) via sigaction!\n", sig);
    printf("[Handler] Unlike signal(), sigaction handlers are NOT reset automatically.\n\n");
}

int main() {
    struct sigaction sa;
    sa.sa_handler = sigint_handler;
    sa.sa_flags = 0;              // No SA_RESTART — scanf will be interrupted
    sigemptyset(&sa.sa_mask);

    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("sigaction failed");
        exit(EXIT_FAILURE);
    }

    printf("\n");
    printf("========================================\n");
    printf("  sigaction() Behavior During scanf\n");
    printf("========================================\n\n");
    printf("[Main PID %d] Using sigaction() instead of signal().\n\n", getpid());
    printf("  Key differences from signal():\n");
    printf("    - Handler is NOT reset after first signal\n");
    printf("    - sa_flags controls restart behavior (SA_RESTART)\n");
    printf("    - sa_mask can block other signals during handler\n\n");
    printf("  Try: press Ctrl+C during scanf, then observe what happens.\n\n");

    int number;
    printf("  Enter a number: ");
    fflush(stdout);

    if (scanf("%d", &number) == 1) {
        printf("\n[Main] scanf succeeded! You entered: %d\n", number);
    } else {
        printf("[Main] scanf failed — signal interrupted the blocking read.\n");
        printf("[Main] With sa.sa_flags = SA_RESTART, scanf would have resumed.\n");
    }
    printf("\n[Main] Program continues normally.\n");
    printf("\n========================================\n");

    return 0;
}
