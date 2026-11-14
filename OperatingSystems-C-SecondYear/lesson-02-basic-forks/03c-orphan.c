/*
 * 03c-orphan.c — When the parent dies first, the child is adopted
 *
 * Key concepts: orphan process, getppid() changes, adoption by PID 1 (init)
 * Compile: gcc -o orphan 03c-orphan.c
 * Run:     ./orphan     (the child's last line appears AFTER the prompt — that is the point)
 *
 * The parent exits after one second. The child is still running: it becomes an ORPHAN
 * and the system's first process (PID 1, "init") becomes its new parent, so it can
 * still be reaped when it ends. The shell already printed the prompt — it only waited
 * for the parent.
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        return EXIT_FAILURE;
    }

    if (pid == 0) {                                   // child
        printf("[Child  %d] my parent is %d\n", getpid(), getppid());
        fflush(stdout);
        sleep(2);                                     // outlive the parent
        printf("\n[Child  %d] my parent is now %d — adopted after my parent exited\n", getpid(), getppid());
        return EXIT_SUCCESS;
    }

    printf("[Parent %d] created child %d, exiting in 1 second without waiting\n", getpid(), pid);
    fflush(stdout);
    sleep(1);
    printf("[Parent %d] bye\n", getpid());
    return EXIT_SUCCESS;
}
