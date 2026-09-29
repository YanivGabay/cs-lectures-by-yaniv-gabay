/*
 * fifo-reader.c — FIFO reader — reading from a named pipe
 *
 * Key concepts: mkfifo, open, read, named pipes persist in filesystem
 * Compile: gcc -o reader fifo-reader.c
 * Run:     ./prog
 */
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
    printf("\n");
    printf("══════════════════════════════════════\n");
    printf("  FIFO Reader (Named Pipe)\n");
    printf("  Run this first, then writer in another terminal\n");
    printf("══════════════════════════════════════\n\n");
    int fd;
    char buffer[BUFFER_SIZE];

    // Create the FIFO (named pipe) if it does not exist
    if (mkfifo(FIFO_NAME, 0666) == -1) {
        if (errno != EEXIST) { // It's okay if the FIFO already exists
            perror("mkfifo");
            exit(EXIT_FAILURE);
        }
    }

    printf("[Reader PID %d] FIFO '%s' created/exists.\n", getpid(), FIFO_NAME);

    // Open the FIFO for reading
    fd = open(FIFO_NAME, O_RDONLY);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    printf("[Reader] Connected (fd=%d). Waiting for messages...\n\n", fd);

    while (1) {
        // Read from the FIFO
        ssize_t bytesRead = read(fd, buffer, BUFFER_SIZE);
        if (bytesRead == -1) {
            perror("read");
            break;
        } else if (bytesRead == 0) {
            // No more writers; exit
            printf("[Reader] read() returned 0 — writer closed the FIFO (EOF).\n");
            break;
        }

        printf("[Reader] Received %ld bytes: \"%s\"\n", (long)bytesRead, buffer);

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
