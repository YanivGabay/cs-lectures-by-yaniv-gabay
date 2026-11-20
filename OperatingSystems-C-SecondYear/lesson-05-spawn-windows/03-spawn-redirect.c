/*
 * 03-spawn-redirect.c — Spawn with the child's output going to a file
 *
 * Key concepts: posix_spawn_file_actions_addopen — the spawn version of fork + dup2 + exec (Lesson 04)
 * Compile: gcc -o calculator calculator.c && gcc -o spawn_redirect 03-spawn-redirect.c
 * Run:     ./spawn_redirect       (then open result.txt in the Files panel)
 *
 * With fork you would open() + dup2() in the child before exec. posix_spawn cannot run your code
 * in the child, so you describe the plan instead: "open result.txt as file descriptor 1 first".
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <spawn.h>
#include <unistd.h>
#include <sys/wait.h>

extern char **environ;

int main(void)
{
    posix_spawn_file_actions_t actions;
    posix_spawn_file_actions_init(&actions);
    // In the child, before the program starts: open result.txt AS stdout (fd 1)
    posix_spawn_file_actions_addopen(&actions, STDOUT_FILENO, "result.txt",
                                     O_WRONLY | O_CREAT | O_TRUNC, 0644);

    char *argv[] = { "calculator", "6", "X", "7", NULL };
    pid_t pid;
    int err = posix_spawn(&pid, "./calculator", &actions, NULL, argv, environ);
    posix_spawn_file_actions_destroy(&actions);
    if (err != 0) {
        fprintf(stderr, "posix_spawn: %s\n", strerror(err));
        return EXIT_FAILURE;
    }

    int status;
    waitpid(pid, &status, 0);
    printf("[Parent] calculator (PID %d) exited with %d; everything it printed is in result.txt.\n",
           pid, WEXITSTATUS(status));
    return EXIT_SUCCESS;
}
