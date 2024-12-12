// fifo_writer.c
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>      // For O_WRONLY
#include <sys/stat.h>   // For mkfifo
#include <unistd.h>     // For write and close
#include <string.h>
#include <errno.h>


// we first run the Reader first.
// we will compile both program
// than run first the Reader
// than run the writer
// for example:
// terminal 1: ./fifo-reader
// terminal 2: ./fifo-writer

//WE use here WRITE AND READ, which yoram didnt showcase with mkfifo
//if we use them we dont need to use fflush,
// but next examples we will be using File* to use the mkfifo and than we will need to use fflush

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

    printf("Writer: FIFO '%s' created/opened for writing.\n", FIFO_NAME);

    // Open the FIFO for writing
    fd = open(FIFO_NAME, O_WRONLY);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    printf("Writer: FIFO opened. Enter messages to send. Type 'exit' to quit.\n");

    while (1) {
        printf("You: ");
        fflush(stdout);

        // Read input from the user
        if (fgets(buffer, BUFFER_SIZE, stdin) == NULL) {
            perror("fgets");
            break;
        }

        // Remove newline character
        buffer[strcspn(buffer, "\n")] = '\0';

        // Exit condition
        if (strcmp(buffer, "exit") == 0) {
            break;
        }

        // Write the message to the FIFO
        if (write(fd, buffer, strlen(buffer) + 1) == -1) { // +1 to include '\0'
            perror("write");
            break;
        }
    }

    // Close the FIFO
    close(fd);
    printf("Writer: Exiting.\n");



 
    return 0;
}
