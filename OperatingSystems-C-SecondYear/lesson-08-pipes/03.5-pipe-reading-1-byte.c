/*
 * 03.5-pipe-reading-1-byte.c — Reading from pipe one byte at a time
 *
 * Key concepts: read() with size 1, byte-by-byte pipe reading, EOF detection
 * Compile: gcc -o byte_pipe 03.5-pipe-reading-1-byte.c
 * Run:     ./prog
 */
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
int main() {
    printf("\n");
    printf("══════════════════════════════════════\n");
    printf("  Byte-by-Byte Pipe Reading\n");
    printf("  read(fd, &c, 1) in a loop\n");
    printf("══════════════════════════════════════\n\n");
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
            printf("[Child  PID %d] Read byte: '%c' (0x%02x)\n", getpid(), c, (unsigned char)c);
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
        printf("[Parent PID %d] Writing \"%s\" one byte at a time...\n", getpid(), message);
        ssize_t len = 0;
        sleep(2);
        while (message[len] != '\0') {
            if (write(my_pipe[1], &message[len], 1) != 1) {
                perror("write");
                exit(EXIT_FAILURE);
            }
            len++;
        }

        printf("[Parent PID %d] Wrote %ld bytes. Closing write end (signals EOF).\n", getpid(), len);
        close(my_pipe[1]); // Close write end to signal EOF
        wait(NULL); // Wait for child to finish
    }

    return 0;
}
