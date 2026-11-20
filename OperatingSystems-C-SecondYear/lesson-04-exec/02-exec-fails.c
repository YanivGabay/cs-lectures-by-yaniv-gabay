/*
 * 02-exec-fails.c — exec returns only when it fails: always check it
 *
 * Key concepts: exec returns -1 and sets errno; ENOENT vs EACCES; perror(); exit after a failed exec
 * Compile: gcc -o exec_fails 02-exec-fails.c
 * Run:     ./exec_fails
 *
 * Two classic mistakes: a program that does not exist (ENOENT), and a file that exists
 * but is not executable — like a .c source file (EACCES).
 */
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

static void try_exec(const char *path)
{
    fflush(stdout);                       // flush before fork, or the child prints our text again (Lesson 02)
    pid_t pid = fork();
    if (pid == 0) {
        execl(path, path, (char *)NULL);
        // Only reached if exec failed:
        printf("  exec(\"%s\") failed: errno=%d (%s)\n", path, errno, strerror(errno));
        exit(127);                        // like the shell: 127 = could not run the command
    }
    int status;
    waitpid(pid, &status, 0);
    printf("  child exit status: %d\n\n", WEXITSTATUS(status));
}

int main(void)
{
    printf("1) A program that does not exist:\n");
    try_exec("./no_such_program");

    printf("2) A file that exists but is not executable (our own source file):\n");
    try_exec("./02-exec-fails.c");

    return EXIT_SUCCESS;
}
