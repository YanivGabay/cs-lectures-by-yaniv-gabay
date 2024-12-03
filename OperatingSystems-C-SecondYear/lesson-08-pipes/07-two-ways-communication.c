#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define BUFFER_SIZE 100

int main() {
    int pipe1_fd[2]; // Pipe for parent -> child
    int pipe2_fd[2]; // Pipe for child -> parent
    pid_t pid;
    char buffer[BUFFER_SIZE];

    // Create first pipe (parent to child)
    if (pipe(pipe1_fd) == -1) {
        perror("pipe1");
        exit(EXIT_FAILURE);
    }

    // Create second pipe (child to parent)
    if (pipe(pipe2_fd) == -1) {
        perror("pipe2");
        exit(EXIT_FAILURE);
    }

    // Fork the process
    pid = fork();
    if (pid == -1) { // Error handling for fork
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) { // Child Process
        // Close unused ends
        close(pipe1_fd[1]); // Close write end of pipe1
        close(pipe2_fd[0]); // Close read end of pipe2

        // Read message from parent
        ssize_t bytesRead = read(pipe1_fd[0], buffer, sizeof(buffer) - 1);
        if (bytesRead == -1) {
            perror("Child read from pipe1");
            exit(EXIT_FAILURE);
        }
        buffer[bytesRead] = '\0'; // Null-terminate the string
        printf("Child received from parent: %s\n", buffer);

        // Prepare response
        const char *child_msg = "Hello from Child!";
        ssize_t bytesWritten = write(pipe2_fd[1], child_msg, strlen(child_msg) + 1);
        if (bytesWritten == -1) {
            perror("Child write to pipe2");
            exit(EXIT_FAILURE);
        }

        // Close used ends
        close(pipe1_fd[0]);
        close(pipe2_fd[1]);

        exit(EXIT_SUCCESS);
    } else { // Parent Process
        // Close unused ends
        close(pipe1_fd[0]); // Close read end of pipe1
        close(pipe2_fd[1]); // Close write end of pipe2

        // Send message to child
        const char *parent_msg = "Hello from Parent!";
        ssize_t bytesWritten = write(pipe1_fd[1], parent_msg, strlen(parent_msg) + 1);
        if (bytesWritten == -1) {
            perror("Parent write to pipe1");
            exit(EXIT_FAILURE);
        }

        // Read response from child
        ssize_t bytesRead = read(pipe2_fd[0], buffer, sizeof(buffer) - 1);
        if (bytesRead == -1) {
            perror("Parent read from pipe2");
            exit(EXIT_FAILURE);
        }
        buffer[bytesRead] = '\0'; // Null-terminate the string
        printf("Parent received from child: %s\n", buffer);

        // Close used ends
        close(pipe1_fd[1]);
        close(pipe2_fd[0]);

        // Wait for child to finish
        if (wait(NULL) == -1) {
            perror("wait");
            exit(EXIT_FAILURE);
        }
    }

    return 0;
}
