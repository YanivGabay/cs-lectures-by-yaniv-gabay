// process_b.c
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>      // For O_WRONLY, O_RDONLY
#include <sys/stat.h>   // For mkfifo
#include <unistd.h>     // For close
#include <string.h>
#include <errno.h>
const char *FIFO_A_TO_B = "fifo_a_to_b";
const char *FIFO_B_TO_A = "fifo_b_to_a";
const int BUFFER_SIZE = 1024;

int main() {
    FILE *fp_read, *fp_write;
    char buffer[BUFFER_SIZE];
    pid_t pid = getpid();

    // Create both FIFOs if they do not exist
    if (mkfifo(FIFO_A_TO_B, 0666) == -1) {
        if (errno != EEXIST) {
            perror("mkfifo fifo_a_to_b");
            exit(EXIT_FAILURE);
        }
    }

    if (mkfifo(FIFO_B_TO_A, 0666) == -1) {
        if (errno != EEXIST) {
            perror("mkfifo fifo_b_to_a");
            exit(EXIT_FAILURE);
        }
    }

    printf("Process B (%d): FIFOs '%s' and '%s' created/opened.\n", pid, FIFO_A_TO_B, FIFO_B_TO_A);

    // Open FIFO_A_TO_B for reading
    fp_read = fopen(FIFO_A_TO_B, "r");
    if (fp_read == NULL) {
        perror("fopen fifo_a_to_b for reading");
        exit(EXIT_FAILURE);
    }

    // Open FIFO_B_TO_A for writing
    fp_write = fopen(FIFO_B_TO_A, "w");
    if (fp_write == NULL) {
        perror("fopen fifo_b_to_a for writing");
        fclose(fp_read);
        exit(EXIT_FAILURE);
    }

    printf("Process B (%d): Communication established.\n", pid);

    while (1) {
        // Read the message from FIFO_A_TO_B
        if (fgets(buffer, BUFFER_SIZE, fp_read) == NULL) {
            perror("fgets from fifo_a_to_b");
            break;
        }

        // Remove newline character
        buffer[strcspn(buffer, "\n")] = '\0';

        printf("Process B (%d): Received -> %s\n", pid, buffer);

        // Exit condition
        if (strcmp(buffer, "exit") == 0) {
            printf("Process B (%d): Exit signal received. Exiting.\n", pid);
            break;
        }

        // Prepare a response
        char response[BUFFER_SIZE];
        snprintf(response, BUFFER_SIZE, "Acknowledged: '%s'", buffer);

        // Write the response to FIFO_B_TO_A
        if (fprintf(fp_write, "%s\n", response) < 0) {
            perror("fprintf to fifo_b_to_a");
            break;
        }

        // Flush to ensure the response is sent immediately
        fflush(fp_write);
    }

    // Close the FIFOs
    fclose(fp_read);
    fclose(fp_write);


        //this will delete the FIFO file and its important to do it!!!
     unlink(FIFO_NAME);
    return 0;
}
