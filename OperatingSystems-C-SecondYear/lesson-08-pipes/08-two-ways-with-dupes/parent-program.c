// parent_program.c
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>


/*
this is a bit confusing example
but we will need to scheme it on the board to better udnerstand

we use two pipes to communicate between parent and child
using dupes , the stdout of the parent goes to the child (to its std in)
and the stdout of the child comes back to the parent (to its std in)


*/


#define BUFFER_SIZE 100

int main() {
    int pipe_parent_to_child[2];
    int pipe_child_to_parent[2];
    pid_t pid;
    char buffer[BUFFER_SIZE];

    // Create pipe for parent to child communication
    if (pipe(pipe_parent_to_child) == -1) {
        perror("pipe_parent_to_child");
        exit(EXIT_FAILURE);
    }

    // Create pipe for child to parent communication
    if (pipe(pipe_child_to_parent) == -1) {
        perror("pipe_child_to_parent");
        exit(EXIT_FAILURE);
    }

    // Fork the process
    pid = fork();
    if (pid == -1) { // Error handling for fork
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) { // Child Process
        // Redirect stdin to read from pipe_parent_to_child
        if (dup2(pipe_parent_to_child[0], STDIN_FILENO) == -1) {
            perror("dup2 stdin");
            exit(EXIT_FAILURE);
        }

        // Redirect stdout to write to pipe_child_to_parent
        if (dup2(pipe_child_to_parent[1], STDOUT_FILENO) == -1) {
            perror("dup2 stdout");
            exit(EXIT_FAILURE);
        }

        // Close unused file descriptors
        close(pipe_parent_to_child[0]);
        close(pipe_parent_to_child[1]);
        close(pipe_child_to_parent[0]);
        close(pipe_child_to_parent[1]);

        // Execute the child program
        execlp("./child_program", "./child_program", (char *)NULL);

        // If exec fails
        perror("execlp");
        exit(EXIT_FAILURE);
    } else { // Parent Process
        // Close unused file descriptors
        close(pipe_parent_to_child[0]); // Close read end of parent to child
        close(pipe_child_to_parent[1]); // Close write end of child to parent

        // Convert file descriptors to FILE* streams for easier I/O
        FILE *write_fp = fdopen(pipe_parent_to_child[1], "w");
        if (write_fp == NULL) {
            perror("fdopen write_fp");
            exit(EXIT_FAILURE);
        }

        FILE *read_fp = fdopen(pipe_child_to_parent[0], "r");
        if (read_fp == NULL) {
            perror("fdopen read_fp");
            exit(EXIT_FAILURE);
        }

        // Send message to child
        const char *message = "Hello from Parent Program!";
        fprintf(write_fp, "%s\n", message);
        fflush(write_fp); // Ensure the message is sent

        // Read response from child
        if (fgets(buffer, sizeof(buffer), read_fp) != NULL) {
            printf("Parent received: %s", buffer);
        }

        // Read second response from child
        if (fgets(buffer, sizeof(buffer), read_fp) != NULL) {
            printf("Parent received: %s", buffer);
        }

        // Close the streams
        fclose(write_fp); // Sends EOF to child
        fclose(read_fp);

        // Wait for child to finish
        if (wait(NULL) == -1) {
            perror("wait");
            exit(EXIT_FAILURE);
        }
    }

    return 0;
}
