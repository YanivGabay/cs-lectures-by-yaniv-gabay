/*
 * Challenge (Lesson 08): write into a pipe nobody reads
 *
 * The child closes its read end and exits. The parent then writes into the pipe.
 * Writing to a pipe with NO reader left raises SIGPIPE, whose default action kills
 * the writer — but right now the write succeeds. Why? The parent still holds a read
 * end itself, so the kernel thinks someone could still read.
 *
 * Your task: make the parent close the end it does not use, so the write hits SIGPIPE.
 * Done when the program is "Killed by SIGPIPE (13)".
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void)
{
    int fd[2];
    if (pipe(fd) == -1) { perror("pipe"); return 1; }

    pid_t pid = fork();
    if (pid == 0) {                 // child: the only reader... who leaves at once
        close(fd[1]);
        close(fd[0]);
        _exit(0);
    }

    // The parent still holds BOTH ends here.
    // TODO: the parent only writes — close the end it does not need

    wait(NULL);                     // the reader is gone now
    printf("Parent: writing into the pipe...\n");
    fflush(stdout);
    const char *msg = "hello?\n";
    ssize_t n = write(fd[1], msg, strlen(msg));
    printf("Parent: write returned %zd — nobody got killed\n", n);
    return 0;
}
