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
    char buffer[100];

    // Open the FIFO for reading
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
    printf("Reader: Message received: %s", buffer);

    close(fifo_fd);
    return 0;
}
