/*
 * 02a-kill-child.c — kill(): one process sends a signal to another
 *
 * Key concepts: kill(pid, sig), SIGTERM, waitpid + WIFSIGNALED/WTERMSIG (Lesson 02)
 * Compile: gcc -o kill_child 02a-kill-child.c
 * Run:     ./kill_child
 *
 * Despite its name, kill() just SENDS a signal. SIGTERM's default action terminates the
 * child, and the parent sees exactly which signal ended it in the waitpid() status.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>    // strsignal
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void)
{
    pid_t child = fork();
    if (child == -1) {
        perror("fork");
        return EXIT_FAILURE;
    }

    if (child == 0) {                                  // child: work forever
        for (int i = 1; ; i++) {
            printf("  [Child %d] working... %d\n", getpid(), i);
            fflush(stdout);
            sleep(1);
        }
    }

    sleep(3);                                          // parent: let it work a little
    printf("[Parent %d] kill(%d, SIGTERM)\n", getpid(), child);
    fflush(stdout);
    if (kill(child, SIGTERM) == -1) {
        perror("kill");
        return EXIT_FAILURE;
    }

    int status;
    waitpid(child, &status, 0);
    if (WIFSIGNALED(status))
        printf("[Parent] child ended by signal %d (%s)\n", WTERMSIG(status), strsignal(WTERMSIG(status)));
    return EXIT_SUCCESS;
}
