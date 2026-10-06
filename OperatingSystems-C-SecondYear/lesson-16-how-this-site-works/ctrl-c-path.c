/*
 * ctrl-c-path.c — Where does Ctrl+C go when you click the Ctrl+C button?
 *
 * Demonstrates: the terminal sends SIGINT to the whole FOREGROUND PROCESS GROUP
 * Key concepts: sigaction(), getpgrp(), tcgetpgrp(), fork() keeps the process group
 * Compile: gcc -Wall -o ctrl-c-path ctrl-c-path.c
 * Run:     ./ctrl-c-path      (then press Ctrl+C, or wait — the site presses it for you)
 *
 * The site's Ctrl+C button does not "kill your program". It types the ^C character into
 * the terminal (exactly like your keyboard). The terminal driver turns ^C into SIGINT and
 * delivers it to every process in the foreground group — so parent AND child both get it.
 */

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>

static const char *who = "Parent";

static void on_sigint(int sig)
{
    /* printf is not async-signal-safe; fine for a demo, use write() in real code */
    printf("[%s PID %d] got signal %d (SIGINT) — I am in process group %d\n",
           who, (int)getpid(), sig, (int)getpgrp());
    fflush(stdout);
    _exit(0);
}

int main(void)
{
    struct sigaction sa;
    sa.sa_handler = on_sigint;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL); /* installed before fork(): the child inherits it (Lesson 06) */

    printf("\n══════════════════════════════════════\n");
    printf("  The path of Ctrl+C\n");
    printf("══════════════════════════════════════\n\n");
    printf("[Parent PID %d] process group %d; the terminal's foreground group is %d\n",
           (int)getpid(), (int)getpgrp(), (int)tcgetpgrp(STDIN_FILENO));
    fflush(stdout);

    if (fork() == 0) {
        who = "Child";
        printf("[Child  PID %d] same process group %d — fork() does not change it\n",
               (int)getpid(), (int)getpgrp());
        fflush(stdout);
        for (;;)
            pause();
    }

    printf("[Parent] Both of us now wait. Press Ctrl+C...\n");
    fflush(stdout);
    for (;;)
        pause();
}
