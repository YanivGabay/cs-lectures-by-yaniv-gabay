/*
 * Challenge (Lessons 03 + 06): survive Ctrl+C
 *
 * This program counts forever. When the site presses Ctrl+C (after 3 seconds),
 * SIGINT's default action kills it.
 *
 * Your task: install a SIGINT handler with sigaction() that prints
 * "Caught Ctrl+C — still running" and lets the program keep counting.
 * Done when the program is still alive after Ctrl+C (the site then stops it with Ctrl+\).
 */
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void on_sigint(int sig) { (void)sig; printf("Caught Ctrl+C — still running\n"); fflush(stdout); }

int main(void)
{
    struct sigaction sa;
    sa.sa_handler = on_sigint;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sigaction(SIGINT, &sa, NULL);

    for (int tick = 1; ; tick++) {
        printf("tick %d\n", tick);
        fflush(stdout);
        sleep(1);
    }
}
