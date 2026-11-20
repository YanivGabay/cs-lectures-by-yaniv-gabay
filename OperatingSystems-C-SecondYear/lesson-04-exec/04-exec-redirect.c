/*
 * 04-exec-redirect.c — Open files survive exec: this is how `ls -l > listing.txt` works
 *
 * Key concepts: file descriptors are kept across exec, open(), dup2(), fork + exec
 * Compile: gcc -o exec_redirect 04-exec-redirect.c
 * Run:     ./exec_redirect       (then open listing.txt in the Files panel)
 *
 * The child opens listing.txt and puts it in place of stdout (file descriptor 1) with dup2().
 * Then it execs `ls -l`. ls has no idea about the file — it just writes to stdout,
 * and stdout is still listing.txt, because exec keeps open file descriptors.
 * (dup2 comes back in depth in Lesson 08.)
 */
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void)
{
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        return EXIT_FAILURE;
    }

    if (pid == 0) {
        int fd = open("listing.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd == -1) {
            perror("listing.txt");
            exit(1);
        }
        dup2(fd, STDOUT_FILENO);          // stdout (fd 1) now refers to listing.txt
        close(fd);
        execlp("ls", "ls", "-l", (char *)NULL);
        perror("execlp");                 // stderr still goes to the terminal
        exit(127);
    }

    int status;
    waitpid(pid, &status, 0);
    printf("[Parent] ls exited with status %d; its output is in listing.txt, not here.\n", WEXITSTATUS(status));
    return EXIT_SUCCESS;
}
