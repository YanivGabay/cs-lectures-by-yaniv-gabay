/*
 * 05-sighandler_behaviour.c — Signal handler behavior during blocking syscalls
 *
 * Key concepts: signal delivery interrupts sleep/scanf, handler re-registration
 * Compile: gcc -o handler 05-sighandler_behaviour.c
 * Run:     ./handler
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>    // for getpid()
#include <signal.h>    // for signal(), SIGINT

void signal_handler(int sig) {
    printf("\n[Handler] Caught signal %d (SIGINT) — Ctrl+C pressed during scanf!\n", sig);
    printf("[Handler] Returning to scanf... (it may fail or resume depending on the OS)\n\n");
}

int main() {
    signal(SIGINT, signal_handler);

    printf("\n");
    printf("========================================\n");
    printf("  signal() Behavior During scanf\n");
    printf("========================================\n\n");
    printf("[Main PID %d] What happens when Ctrl+C arrives while scanf() is blocking?\n\n", getpid());
    printf("  Experiment:\n");
    printf("    1. Don't type anything yet\n");
    printf("    2. Press Ctrl+C to send SIGINT\n");
    printf("    3. Observe: does scanf resume or fail?\n\n");

    int number;
    printf("  Enter a number: ");
    fflush(stdout);

    if (scanf("%d", &number) == 1) {
        printf("\n[Main] scanf succeeded! You entered: %d\n", number);
    } else {
        printf("[Main] scanf returned 0 or EOF — the signal interrupted the read.\n");
        printf("[Main] This is why sigaction() with SA_RESTART is often preferred.\n");
    }
    printf("\n[Main] Program continues normally after scanf.\n");
    printf("\n========================================\n");

    return 0;
}
