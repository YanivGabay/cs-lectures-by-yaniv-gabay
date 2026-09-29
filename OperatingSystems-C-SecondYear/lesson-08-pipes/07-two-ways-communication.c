/*
 * 07-two-ways-communication.c — Bidirectional IPC using two pipes
 *
 * Demonstrates: Two-way parent-child communication with a pair of pipes
 * Key concepts: One pipe per direction (pipes are unidirectional!), fd cleanup
 * Compile: gcc -o two_way 07-two-ways-communication.c
 * Run:     ./two_way
 *
 * Architecture:
 *   pipe1: Parent --write--> pipe1_fd[1] ... pipe1_fd[0] --read--> Child
 *   pipe2: Child  --write--> pipe2_fd[1] ... pipe2_fd[0] --read--> Parent
 */

#include <unistd.h>    // for pipe(), fork(), read(), write(), close()
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> // for pid_t
#include <sys/wait.h>  // for wait()

#define BUFFER_SIZE 100

int main() {
    printf("\n");
    printf("══════════════════════════════════════\n");
    printf("  Bidirectional IPC — Two Pipes\n");
    printf("  pipe1: Parent→Child, pipe2: Child→Parent\n");
    printf("══════════════════════════════════════\n\n");
    int pipe1_fd[2]; // Pipe 1: parent writes, child reads
    int pipe2_fd[2]; // Pipe 2: child writes, parent reads
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
        printf("[Child  PID %d] Received from parent via pipe1: \"%s\"\n", getpid(), buffer);

        // Prepare response
        printf("[Child  PID %d] Sending reply via pipe2...\n", getpid());
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
        printf("[Parent PID %d] Sending message to child via pipe1...\n", getpid());
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
        printf("[Parent PID %d] Received from child via pipe2: \"%s\"\n", getpid(), buffer);

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
