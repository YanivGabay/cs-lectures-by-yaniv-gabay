/*
 * 01-posix-spawn.c — The Linux "spawn": fork + exec in one call (compare main.c on Windows)
 *
 * Key concepts: posix_spawn / posix_spawnp, waitpid right away (like _P_WAIT) or later (_P_NOWAIT)
 * Compile: gcc -o calculator calculator.c && gcc -o posix_spawn 01-posix-spawn.c
 * Run:     ./posix_spawn
 *
 *   Windows (main.c)                      Linux (this file)
 *   _spawnl(_P_WAIT, "./calculator", …)   posix_spawn(&pid, "./calculator", …) + waitpid(pid)
 *   _spawnlp(…, "calculator", …)          posix_spawnp: searches PATH, like execvp
 *   _P_NOWAIT                             posix_spawn, do other work, waitpid later
 *
 * posix_spawn takes an argv ARRAY (there is no "l" list form) and returns 0 or an error number.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <spawn.h>
#include <unistd.h>
#include <sys/wait.h>

extern char **environ;                    // pass our environment on to the child

static pid_t spawn_calc(char *a, char *op, char *b)
{
    char *argv[] = { "calculator", a, op, b, NULL };
    pid_t pid;
    int err = posix_spawn(&pid, "./calculator", NULL, NULL, argv, environ);
    if (err != 0) {                       // returns the error number itself (not -1/errno)
        fprintf(stderr, "posix_spawn: %s\n", strerror(err));
        exit(EXIT_FAILURE);
    }
    return pid;
}

int main(void)
{
    int status;

    printf("1) Spawn and wait right away (like _P_WAIT):\n");
    fflush(stdout);
    pid_t pid = spawn_calc("5", "+", "3");
    waitpid(pid, &status, 0);
    printf("   -> calculator exited with status %d\n\n", WEXITSTATUS(status));

    printf("2) Spawn, keep working, wait later (like _P_NOWAIT):\n");
    fflush(stdout);
    pid = spawn_calc("8", "X", "4");
    printf("   [Parent] spawned PID %d and kept going without waiting\n", pid);
    fflush(stdout);
    waitpid(pid, &status, 0);
    printf("   -> collected it later, status %d\n\n", WEXITSTATUS(status));

    printf("3) posix_spawnp searches PATH (like _spawnlp / execvp):\n");
    fflush(stdout);
    char *argv[] = { "echo", "   hello from echo, found via PATH", NULL };
    if (posix_spawnp(&pid, "echo", NULL, NULL, argv, environ) == 0)
        waitpid(pid, &status, 0);
    return EXIT_SUCCESS;
}
