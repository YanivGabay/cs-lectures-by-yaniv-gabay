/*
 * 07-before-ex2.c — Practical alarm example — preparation for exercise 2
 *
 * Key concepts: alarm(), sigaction, practical signal usage
 * Compile: gcc -o ex2 07-before-ex2.c
 * Run:     ./prog
 */
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>


int global_flag = 1;

void handle_sigalrm(int signum) {
    printf("\n[Handler] Time's up! Caught SIGALRM (signal %d).\n", signum);
    global_flag = 0;
}

int main() {
    printf("\n");
    printf("══════════════════════════════════════\n");
    printf("  Timed Input with alarm() + sigaction\n");
    printf("  Enter numbers before the timer runs out!\n");
    printf("══════════════════════════════════════\n\n");
    const int seconds = 5;
    struct sigaction sa;

    sa.sa_handler = handle_sigalrm;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);

    if (sigaction(SIGALRM, &sa, NULL) == -1) {
        perror("Error: sigaction for SIGALRM failed");
        exit(EXIT_FAILURE);
    }

    printf("[Main] SIGALRM handler registered.\n");
    printf("[Main] alarm(%d) gives you %d seconds per round.\n\n", seconds, seconds);

    int best = 0;
    for (int i = 0; i < 10; i++) {
        global_flag = 1;
        int counter = 0;
        printf("── Round %d/10 ──────────────────────\n", i + 1);
        while(global_flag)
        {
            alarm(seconds);
            printf("  Enter a number (%ds timer): ", seconds);
            fflush(stdout);
            int number;
            if (scanf("%d", &number) == 1) {
                printf("  Got: %d (entry #%d)\n", number, counter + 1);
            }
            counter++;
        }
        printf("[Main] Round %d: you entered %d numbers before timeout.\n\n", i + 1, counter);
        if(counter > best)
        {
            best = counter;
        }
    }

    printf("══════════════════════════════════════\n");
    printf("  Best round: %d entries before timeout\n", best);
    printf("══════════════════════════════════════\n");

    return 0;
}
