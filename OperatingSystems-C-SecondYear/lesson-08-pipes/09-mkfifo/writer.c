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
    const char *message = "Hello from the Writer!\n";

    // Open the FIFO for writing
    int fifo_fd = open(FIFO_PATH, O_WRONLY);
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

    printf("Writer: Message written to FIFO.\n");

    close(fifo_fd);
    return 0;
}
