/*
 * 05-mini-shell.c — A tiny shell: read a command, fork, exec it, wait, report how it ended
 *
 * Key concepts: fgets + strtok into an argv array, fork + execvp + waitpid, exit status
 * Compile: gcc -o mini_shell 05-mini-shell.c
 * Run:     ./mini_shell      (try: echo hello · ls -l · wc -l 05-mini-shell.c · nosuchcmd · exit)
 *
 * This is exactly what bash does for every command you type — minus pipes, redirection,
 * quotes and the rest. execvp searches PATH, so "ls" works without "/bin/ls".
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_ARGS 32

int main(void)
{
    char line[256];

    for (;;) {
        printf("mini$ ");
        fflush(stdout);
        if (fgets(line, sizeof line, stdin) == NULL)   // Ctrl+D (end of input) ends the shell
            break;

        // Split the line into words: "wc -l file" -> {"wc", "-l", "file", NULL}
        char *argv[MAX_ARGS];
        int argc = 0;
        for (char *w = strtok(line, " \t\n"); w && argc < MAX_ARGS - 1; w = strtok(NULL, " \t\n"))
            argv[argc++] = w;
        argv[argc] = NULL;
        if (argc == 0)
            continue;                                    // empty line
        if (strcmp(argv[0], "exit") == 0)
            break;

        pid_t pid = fork();
        if (pid == -1) {
            perror("fork");
            continue;
        }
        if (pid == 0) {
            execvp(argv[0], argv);
            fprintf(stderr, "mini: %s: command not found\n", argv[0]);
            exit(127);
        }

        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status))
            printf("[mini] %s exited with status %d\n", argv[0], WEXITSTATUS(status));
        else if (WIFSIGNALED(status))
            printf("[mini] %s killed by signal %d\n", argv[0], WTERMSIG(status));
    }
    printf("bye\n");
    return EXIT_SUCCESS;
}
