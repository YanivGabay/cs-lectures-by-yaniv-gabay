/*
 * 05-pipe-with-fd.c — Using pipes with file descriptor operations
 *
 * Key concepts: pipe(), file descriptors, read/write
 * Compile: gcc -o pipe_fd 05-pipe-with-fd.c
 * Run:     ./prog
 */
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

int main() {
    printf("\n");
    printf("══════════════════════════════════════\n");
    printf("  Pipe with FILE* (fdopen)\n");
    printf("  Use fprintf/fscanf instead of raw read/write\n");
    printf("══════════════════════════════════════\n\n");
    int pipefd[2];
    pid_t pid;
    char buffer[100];
    FILE *read_fp, *write_fp;

    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid = fork();
    if (pid == 0) { // Child: Reader
        close(pipefd[1]); // Close write end
        read_fp = fdopen(pipefd[0], "r");
        if (read_fp == NULL) {
            perror("fdopen");
            exit(EXIT_FAILURE);
        }
        fscanf(read_fp, "%99s", buffer);
        printf("[Child  PID %d] Read via FILE*/fscanf: \"%s\"\n", getpid(), buffer);
        fclose(read_fp);
        exit(EXIT_SUCCESS);
    } else { // Parent: Writer
        close(pipefd[0]); // Close read end
        printf("[Parent PID %d] Writing via FILE*/fprintf...\n", getpid());
        write_fp = fdopen(pipefd[1], "w");
        if (write_fp == NULL) {
            perror("fdopen");
            exit(EXIT_FAILURE);
        }
        fprintf(write_fp, "Hello_File_Stream\n");
        fclose(write_fp);
    }

    return 0;
}
