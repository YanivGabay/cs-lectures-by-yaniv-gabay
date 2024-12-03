#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 1024

int main() {
    int pipefd[2];
    pid_t pid;
    ssize_t bytes;
    char buffer[BUFFER_SIZE];

    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid = fork();
    if (pid == 0) { // Child: Reader
        close(pipefd[1]); // Close write end
        while ((bytes = read(pipefd[0], buffer, sizeof(buffer)-1)) > 0) {
            buffer[bytes] = '\0';
            printf("Child read: %s", buffer);
        }
        close(pipefd[0]);
        exit(EXIT_SUCCESS);
    } else { // Parent: Writer
        close(pipefd[0]); // Close read end
        const char *msg = "Hello, this is a message from the parent.\n";
        size_t len = strlen(msg);
        ssize_t total_written = 0;

        while (total_written < len) {
            bytes = write(pipefd[1], msg + total_written, len - total_written);
            if (bytes == -1) {
                perror("write");
                break;
            }
            total_written += bytes;
        }
        close(pipefd[1]);
    }

    return 0;
}
