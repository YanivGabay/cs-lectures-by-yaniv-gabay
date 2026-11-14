/*
 * 03a-wait-status.c — How did my child end? Decoding the status from waitpid()
 *
 * Key concepts: waitpid(), WIFEXITED/WEXITSTATUS, WIFSIGNALED/WTERMSIG, exit codes
 * Compile: gcc -o wait_status 03a-wait-status.c
 * Run:     ./wait_status
 *
 * Three children end in three different ways. The parent learns how each one ended from
 * the status integer — the same information the site turns into "Exited 42" or
 * "Killed by SIGSEGV (11)", and that the shell keeps in $?.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>    // strsignal
#include <unistd.h>
#include <sys/wait.h>

int main(void)
{
    pid_t kids[3];

    for (int i = 0; i < 3; i++) {
        kids[i] = fork();
        if (kids[i] == -1) {
            perror("fork");
            return EXIT_FAILURE;
        }
        if (kids[i] == 0) {               // child i
            if (i == 0)
                exit(0);                  // ends normally
            if (i == 1)
                exit(42);                 // ends with an error code
            volatile int *p = NULL;       // child 2 crashes on purpose
            *p = 1;
        }
    }

    for (int i = 0; i < 3; i++) {
        int status;
        waitpid(kids[i], &status, 0);     // wait for THIS child
        printf("Child %d (PID %d): ", i, kids[i]);
        if (WIFEXITED(status))
            printf("exited with status %d\n", WEXITSTATUS(status));
        else if (WIFSIGNALED(status))
            printf("killed by signal %d (%s)\n", WTERMSIG(status), strsignal(WTERMSIG(status)));
    }
    return EXIT_SUCCESS;
}
