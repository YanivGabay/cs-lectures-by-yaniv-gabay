/*
 * 03-sig_alarm.c — Using alarm() for a countdown timer
 *
 * Demonstrates: SIGALRM delivery after a timed delay
 * Key concepts: alarm(seconds) schedules a SIGALRM, only one alarm active at a time
 * Compile: gcc -o alarm 03-sig_alarm.c
 * Run:     ./alarm   (waits 5 seconds, then prints and exits)
 */

#include <stdio.h>
#include <stdlib.h>    // for exit()
#include <unistd.h>    // for alarm()
#include <signal.h>    // for signal(), SIGALRM

void handle_alarm(int sig);

int main() {
    int countdown = 5;

    // Register the signal handler
    signal(SIGALRM, handle_alarm);

    // Set the alarm for 5 seconds
    printf("\n");
    printf("========================================\n");
    printf("  SIGALRM — Timer Countdown\n");
    printf("========================================\n\n");
    printf("[Main PID %d] Setting alarm(%d) — SIGALRM will fire in %d seconds.\n\n", getpid(), countdown, countdown);
    alarm(countdown);

    printf("  Waiting for the alarm...\n");
    fflush(stdout);
    while (1) {
        sleep(1);
    }

    return 0;
}

void handle_alarm(int sig) {
    printf("\n[Signal Handler] SIGALRM received (signal %d) — time's up! Exiting.\n", sig);
    exit(0); // Exit the program
}