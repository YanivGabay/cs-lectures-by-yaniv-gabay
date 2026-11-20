/*
 * 02-args-shell-vs-spawn.c — Who splits "aa bb cc" and who expands * ? (the Windows quirks, on Linux)
 *
 * Key concepts: argv passes through spawn/exec untouched; the SHELL splits words and expands *
 * Compile: gcc -o show_args show_args.c && gcc -o args_demo 02-args-shell-vs-spawn.c
 * Run:     ./args_demo
 *
 * On Windows the program gets ONE command-line string and its C runtime re-splits it and
 * expands * (that was the "two days" bug in main.c). On Linux the argv array arrives exactly
 * as passed — splitting and * expansion happen only when a SHELL reads a command line,
 * for example through system().
 */
#include <stdio.h>
#include <stdlib.h>
#include <spawn.h>
#include <sys/wait.h>

extern char **environ;

int main(void)
{
    char *argv[] = { "show_args", "aa bb cc", "*", NULL };
    pid_t pid;
    int status;

    printf("1) posix_spawn with the array {\"aa bb cc\", \"*\"} — no shell involved:\n");
    fflush(stdout);
    posix_spawn(&pid, "./show_args", NULL, NULL, argv, environ);
    waitpid(pid, &status, 0);

    printf("\n2) system(\"./show_args aa bb cc *\") — /bin/sh reads it as a command line:\n");
    fflush(stdout);
    system("./show_args aa bb cc *");     // split into words, and * becomes the file names here

    printf("\n3) Same through the shell, but quoted:\n");
    fflush(stdout);
    system("./show_args \"aa bb cc\" '*'");
    return EXIT_SUCCESS;
}
