#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

int main() {
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
        printf("Child received via FILE*: %s\n", buffer);
        fclose(read_fp);
        exit(EXIT_SUCCESS);
    } else { // Parent: Writer
        close(pipefd[0]); // Close read end
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
