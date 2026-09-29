/*
 * reader.c — Named pipe reader — intro to mkfifo (FIFO)
 *
 * Key concepts: mkfifo, open, read from named pipe
 * Compile: gcc -o reader reader.c
 * Run:     ./prog
 */
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define FIFO_PATH "/tmp/my_fifo"
// FIRST create the fifo like:
// terminal -> mkfifo /tmp/my_fifo
// than we can open 2 terminals
// terminal 1: start the reader: (reading blocks remebebr!?!?!) ./reader
// terminal 2: start the writer ./writer


int main() {
    printf("\n");
    printf("══════════════════════════════════════\n");
    printf("  Named Pipe (FIFO) Reader\n");
    printf("  First: mkfifo %s\n", FIFO_PATH);
    printf("══════════════════════════════════════\n\n");
    char buffer[100];

    printf("[Reader PID %d] Opening FIFO %s for reading...\n", getpid(), FIFO_PATH);
    printf("[Reader] (blocks until a writer opens the other end)\n");
    fflush(stdout);
    int fifo_fd = open(FIFO_PATH, O_RDONLY);
    if (fifo_fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    // Read the message from the FIFO
    ssize_t bytesRead = read(fifo_fd, buffer, sizeof(buffer) - 1);
    if (bytesRead == -1) {
        perror("read");
        close(fifo_fd);
        exit(EXIT_FAILURE);
    }

    buffer[bytesRead] = '\0'; // Null-terminate the string
    printf("[Reader PID %d] Received %ld bytes: \"%s\"\n", getpid(), (long)bytesRead, buffer);

    close(fifo_fd);
    return 0;
}
