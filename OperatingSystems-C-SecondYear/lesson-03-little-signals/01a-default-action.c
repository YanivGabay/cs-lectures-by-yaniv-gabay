/*
 * 01a-default-action.c — No handler installed? Then the signal's DEFAULT action happens
 *
 * Key concepts: default actions, raise(), SIGUSR1, why a process "dies" from a signal
 * Compile: gcc -o default_action 01a-default-action.c
 * Run:     ./default_action      (look at the exit chip: "Killed by SIGUSR1")
 *
 * Every signal has a default action: most terminate the process (SIGINT, SIGTERM, SIGUSR1...),
 * some also dump core (SIGSEGV, SIGQUIT), SIGCHLD is ignored, SIGSTOP stops it.
 * raise(sig) sends a signal to the calling process itself.
 */
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

int main(void)
{
    printf("[PID %d] No handler for SIGUSR1 is installed.\n", getpid());
    printf("[PID %d] Sending SIGUSR1 to myself with raise()...\n", getpid());
    fflush(stdout);                        // the default action ends us without flushing

    raise(SIGUSR1);                        // default action for SIGUSR1: terminate

    printf("This line is never printed.\n");
    return 0;
}
