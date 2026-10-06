/*
 * fork-until-eagain.c — A *safe* fork bomb: why the course site survives one
 *
 * Demonstrates: RLIMIT_NPROC stops fork() with EAGAIN — the system protects itself
 * Key concepts: fork() return value -1, errno, EAGAIN, pause(), kill(), wait()
 * Compile: gcc -Wall -o fork-until-eagain fork-until-eagain.c
 * Run:     ./fork-until-eagain
 *
 * Lesson 02 showed fork() in a loop going wrong. Here we do it on purpose — but every
 * child just waits, and the parent cleans up. The limit (64 processes for your user,
 * set by sandbox-launch.c) is reached long before the machine is in danger.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/resource.h>

#define MAX_CHILDREN 1000

int main(void)
{
    pid_t children[MAX_CHILDREN];
    int count = 0;
    struct rlimit rl;
    getrlimit(RLIMIT_NPROC, &rl);

    printf("\n══════════════════════════════════════\n");
    printf("  fork() until the kernel says no\n");
    printf("══════════════════════════════════════\n\n");
    printf("[Parent PID %d] My user may own at most %llu processes (RLIMIT_NPROC).\n",
           (int)getpid(), (unsigned long long)rl.rlim_cur);
    fflush(stdout); /* flush before fork, or every child inherits the buffered text */

    while (count < MAX_CHILDREN) {
        pid_t pid = fork();
        if (pid == -1) {
            /* The interesting line: which error stopped us? */
            printf("[Parent] fork() #%d failed: errno=%d (%s)%s\n", count + 1, errno, strerror(errno),
                   errno == EAGAIN ? " — the process limit did its job" : "");
            break;
        }
        if (pid == 0) {
            pause();   /* child: sleep until a signal arrives */
            _exit(0);
        }
        children[count++] = pid;
    }

    printf("[Parent] Created %d children before the limit (the shell and I count too).\n", count);

    /* Clean up: every child gets SIGTERM, then we collect them so none stay zombies */
    for (int i = 0; i < count; i++)
        kill(children[i], SIGTERM);
    int reaped = 0;
    while (wait(NULL) > 0)
        reaped++;
    printf("[Parent] Sent SIGTERM to all of them and reaped %d with wait(). Nothing left behind.\n", reaped);
    return 0;
}
