/*
 * 03-exec-env.c — The new program gets exactly the environment you pass it
 *
 * Key concepts: environment variables, execle(), getenv(), the env command
 * Compile: gcc -o exec_env 03-exec-env.c
 * Run:     ./exec_env
 *
 * Normally exec passes your environment along unchanged. The 'e' variants (execle, execve)
 * let you hand over a different one — here a tiny environment with just two variables.
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void)
{
    printf("[Parent] my HOME is %s and my USER is %s\n\n", getenv("HOME"), getenv("USER"));
    fflush(stdout);

    pid_t pid = fork();
    if (pid == 0) {
        char *small_env[] = { "GREETING=hello from the parent", "COURSE=OS", NULL };
        printf("[Child] exec'ing `env` with only two variables:\n");
        fflush(stdout);
        execle("/usr/bin/env", "env", (char *)NULL, small_env);
        perror("execle");
        exit(127);
    }
    wait(NULL);
    printf("\n[Parent] env printed only GREETING and COURSE — no HOME, no PATH.\n");
    return EXIT_SUCCESS;
}
