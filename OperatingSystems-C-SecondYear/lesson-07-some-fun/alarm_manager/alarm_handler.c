/*
 * alarm_handler.c — Alarm handler child (executed by alarm_manager)
 *
 * Key concepts: Child process that handles SIGALRM, called via exec
 * Compile: gcc -o alarm_handler alarm_handler.c
 * Run:     ./prog
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>


int main(int argc, char *argv[]) {
    printf("\n");
    printf("══════════════════════════════════════\n");
    printf("  Alarm Handler (Child Process)\n");
    printf("══════════════════════════════════════\n\n");
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <seconds> <parent_pid>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int seconds = atoi(argv[1]);
    pid_t parent_pid = atoi(argv[2]);

    printf("[Handler PID %d] Will sleep %d seconds, then signal parent PID %d\n", getpid(), seconds, parent_pid);
    fflush(stdout);

    // Sleep for the specified number of seconds
    sleep(seconds);

    // Send SIGUSR1 to the parent process
    if (kill(parent_pid, SIGUSR1) == -1) {
        perror("[Handler] Failed to send SIGUSR1 to parent");
        exit(EXIT_FAILURE);
    }

    printf("[Handler PID %d] Sent SIGUSR1 to parent PID %d after %d seconds.\n", getpid(), parent_pid, seconds);

    return 0;
}
