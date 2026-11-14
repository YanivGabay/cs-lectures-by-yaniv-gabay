/*
 * 03b-zombie-live.c — See a zombie with your own eyes, then make it go away
 *
 * Key concepts: zombie (defunct) process, wait() reaps it, ps STAT column
 * Compile: gcc -o zombie 03b-zombie-live.c
 * Run:     ./zombie
 *
 * A child that has finished but was not yet wait()ed for stays in the process table
 * as a ZOMBIE (STAT Z, "<defunct>"): the kernel keeps its exit status for the parent.
 * wait() collects the status — and the zombie disappears.
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

static void show(pid_t pid)
{
    char cmd[64];
    snprintf(cmd, sizeof cmd, "ps -o pid,ppid,stat,comm -p %d", (int)pid);
    fflush(stdout);
    if (system(cmd) != 0)                 // ps exits 1 when the PID no longer exists
        printf("  (PID %d is gone from the process table)\n", (int)pid);
}

int main(void)
{
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        return EXIT_FAILURE;
    }
    if (pid == 0)
        exit(7);                          // child: finish immediately

    sleep(1);                             // parent: do NOT wait yet
    printf("Child %d has exited, but nobody called wait():\n", (int)pid);
    show(pid);

    int status;
    wait(&status);
    printf("\nAfter wait() (child's exit status: %d):\n", WEXITSTATUS(status));
    show(pid);
    return EXIT_SUCCESS;
}
