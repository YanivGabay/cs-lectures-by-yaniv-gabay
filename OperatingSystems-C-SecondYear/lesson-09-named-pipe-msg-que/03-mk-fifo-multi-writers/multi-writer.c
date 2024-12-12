// fifo_writer_multi.c
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>      // For O_WRONLY
#include <sys/stat.h>   // For mkfifo
#include <unistd.h>     // For close
#include <string.h>
#include <errno.h>

const char *FIFO_NAME = "multi_fifo";
const int BUFFER_SIZE = 1024;

int main(int argc, char *argv[]) {
    FILE *fp;
    char buffer[BUFFER_SIZE];
    pid_t pid = getpid();

    // Create the FIFO (named pipe) if it does not exist
    if (mkfifo(FIFO_NAME, 0666) == -1) {
        if (errno != EEXIST) { // It's okay if the FIFO already exists
            perror("mkfifo");
            exit(EXIT_FAILURE);
        }
    }

    printf("Writer %d: FIFO '%s' created/opened for writing.\n", pid, FIFO_NAME);

    // Open the FIFO for writing using fopen
    fp = fopen(FIFO_NAME, "w");
    if (fp == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    printf("Writer %d: FIFO opened. Enter messages to send. Type 'exit' to quit.\n", pid);

    while (1) {
        printf("Writer %d: ", pid);
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
            // Optionally, send a special message or just exit
            //send to the reader that we are exiting
            fprintf(fp, "Writer %d: Exiting.\n", pid);
            break;
        }

        // Write the message to the FIFO using fprintf
        if (fprintf(fp, "%s\n", buffer) < 0) {
            perror("fprintf");
            break;
        }

        // Flush to ensure the message is sent immediately
        fflush(fp);
    }

    // Close the FIFO
    fclose(fp);
    printf("Writer %d: Exiting.\n", pid);

    return 0;
}
