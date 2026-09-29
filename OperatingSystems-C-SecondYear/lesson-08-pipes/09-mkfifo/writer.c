/*
 * writer.c — Named pipe writer — writing to a FIFO
 *
 * Key concepts: open, write to named pipe, FIFO blocks until reader connects
 * Compile: gcc -o writer writer.c
 * Run:     ./prog
 */
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

// FIRST create the fifo like:
// terminal -> mkfifo /tmp/my_fifo
// than we can open 2 terminals
// terminal 1: start the reader: (reading blocks remebebr!?!?!) ./reader
// terminal 2: start the writer ./writer



#define FIFO_PATH "/tmp/my_fifo"

int main() {
    printf("\n");
    printf("══════════════════════════════════════\n");
    printf("  Named Pipe (FIFO) Writer\n");
    printf("  First: mkfifo %s\n", FIFO_PATH);
    printf("══════════════════════════════════════\n\n");
    const char *message = "Hello from the Writer!\n";

    printf("[Writer PID %d] Opening FIFO %s for writing...\n", getpid(), FIFO_PATH);
    printf("[Writer] (blocks until a reader opens the other end)\n");
    fflush(stdout);
    int fifo_fd = open(FIFO_PATH, O_WRONLY);
    // you can also you File* fifo_fd = fopen(FIFO_PATH, "w");
    if (fifo_fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    // Write the message to the FIFO
    if (write(fifo_fd, message, sizeof(char) * strlen(message)) == -1) {
        perror("write");
        close(fifo_fd);
        exit(EXIT_FAILURE);
    }

    printf("[Writer PID %d] Wrote %ld bytes to FIFO: \"%s\"\n", getpid(), (long)strlen(message), "Hello from the Writer!");

    close(fifo_fd);
    return 0;
}
