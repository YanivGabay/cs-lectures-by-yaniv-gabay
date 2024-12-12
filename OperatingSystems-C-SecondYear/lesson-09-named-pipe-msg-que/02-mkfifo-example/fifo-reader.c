// fifo_reader.c
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>      // For O_RDONLY
#include <sys/stat.h>   // For mkfifo
#include <unistd.h>     // For read and close
#include <string.h>
#include <errno.h>

const char *FIFO_NAME = "my_fifo";
const int BUFFER_SIZE = 1024;

int main() {
    int fd;
    char buffer[BUFFER_SIZE];

    // Create the FIFO (named pipe) if it does not exist
    if (mkfifo(FIFO_NAME, 0666) == -1) {
        if (errno != EEXIST) { // It's okay if the FIFO already exists
            perror("mkfifo");
            exit(EXIT_FAILURE);
        }
    }

    printf("Reader: FIFO '%s' created/opened for reading.\n", FIFO_NAME);

    // Open the FIFO for reading
    fd = open(FIFO_NAME, O_RDONLY);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    printf("Reader: FIFO opened. Waiting for messages...\n");

    while (1) {
        // Read from the FIFO
        ssize_t bytesRead = read(fd, buffer, BUFFER_SIZE);
        if (bytesRead == -1) {
            perror("read");
            break;
        } else if (bytesRead == 0) {
            // No more writers; exit
            printf("Reader: No more writers. Exiting.\n");
            break;
        }

        printf("Received: %s\n", buffer);

        // Exit condition
        if (strcmp(buffer, "exit") == 0) {
            printf("Reader: Exit signal received. Exiting.\n");
            break;
        }
    }

    // Close the FIFO
    close(fd);
    //this will delete the FIFO file and its important to do it!!!
    // ONLY ONE PROCESS SHOULD DO THIS
    if(unlink(FIFO_NAME) == -1){
        perror("unlink");
        exit(EXIT_FAILURE);
    }
    return 0;
}
