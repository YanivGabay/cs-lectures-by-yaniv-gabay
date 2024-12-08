#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
int main() {
    int my_pipe[2];
    pid_t pid;
    char c;

    // Create the pipe
    if (pipe(my_pipe) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    // Fork the process
    pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) { // Child Process: Reader
        close(my_pipe[1]); // Close unused write end

        ssize_t bytesRead;
        while ((bytesRead = read(my_pipe[0], &c, 1)) > 0) {
            // Example processing: Print each character
            printf("Child received: %c\n", c);
        }

        if (bytesRead == -1) {
            perror("read");
            exit(EXIT_FAILURE);
        }

        close(my_pipe[0]); // Close read end
        exit(EXIT_SUCCESS);
    } else { // Parent Process: Writer
        close(my_pipe[0]); // Close unused read end

        const char *message = "Hello, Pipe!";
        ssize_t len = 0;
        while (message[len] != '\0') {
            if (write(my_pipe[1], &message[len], 1) != 1) {
                perror("write");
                exit(EXIT_FAILURE);
            }
            len++;
        }

        close(my_pipe[1]); // Close write end to signal EOF
        wait(NULL); // Wait for child to finish
    }

    return 0;
}
