// fifo_reader_single.c
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>      // For O_RDONLY
#include <sys/stat.h>   // For mkfifo
#include <unistd.h>     // For close
#include <string.h>
#include <errno.h>

const char *FIFO_NAME = "multi_fifo";
const int BUFFER_SIZE = 1024;

int main() {
    FILE *fp;
    char buffer[BUFFER_SIZE];
    int writer_count = 0;

    // Create the FIFO (named pipe) if it does not exist
    if (mkfifo(FIFO_NAME, 0666) == -1) {
        if (errno != EEXIST) { // It's okay if the FIFO already exists
            perror("mkfifo");
            exit(EXIT_FAILURE);
        }
    }

    printf("Reader: FIFO '%s' created/opened for reading.\n", FIFO_NAME);

    // Open the FIFO for reading using fopen
    fp = fopen(FIFO_NAME, "r");
    if (fp == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    printf("Reader: FIFO opened. Waiting for messages...\n");

    while (fgets(buffer, BUFFER_SIZE, fp) != NULL) {
        // Remove newline character
        buffer[strcspn(buffer, "\n")] = '\0';

        printf("Reader: Received -> %s\n", buffer);

        // Optional: Exit condition if a specific message is received
        if (strcmp(buffer, "exit") == 0) {
            printf("Reader: Exit signal received. Exiting.\n");
            break;
        }
    }

    // Close the FIFO
    fclose(fp);



        //this will delete the FIFO file and its important to do it!!!
     unlink(FIFO_NAME);
    return 0;
}
