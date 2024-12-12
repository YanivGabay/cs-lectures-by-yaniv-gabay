// process_a.c
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
    FILE *fp_write, *fp_read;
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

    printf("Process A (%d): FIFOs '%s' and '%s' created/opened.\n", pid, FIFO_A_TO_B, FIFO_B_TO_A);

    // Open FIFO_A_TO_B for writing
    fp_write = fopen(FIFO_A_TO_B, "w");
    if (fp_write == NULL) {
        perror("fopen fifo_a_to_b for writing");
        exit(EXIT_FAILURE);
    }

    // Open FIFO_B_TO_A for reading
    fp_read = fopen(FIFO_B_TO_A, "r");
    if (fp_read == NULL) {
        perror("fopen fifo_b_to_a for reading");
        fclose(fp_write);
        exit(EXIT_FAILURE);
    }

    printf("Process A (%d): Communication established. Type 'exit' to quit.\n", pid);

    while (1) {
        printf("Process A: You: ");
        fflush(stdout);

        // Read input from the user
        if (fgets(buffer, BUFFER_SIZE, stdin) == NULL) {
            perror("fgets");
            break;
        }

        // Remove newline character
        buffer[strcspn(buffer, "\n")] = '\0';

        // Write the message to FIFO_A_TO_B
        if (fprintf(fp_write, "%s\n", buffer) < 0) {
            perror("fprintf to fifo_a_to_b");
            break;
        }

        // Flush to ensure the message is sent immediately
        fflush(fp_write);

        // Exit condition
        if (strcmp(buffer, "exit") == 0) {
            break;
        }

        // Read the response from FIFO_B_TO_A
        if (fgets(buffer, BUFFER_SIZE, fp_read) == NULL) {
            perror("fgets from fifo_b_to_a");
            break;
        }

        // Remove newline character
        buffer[strcspn(buffer, "\n")] = '\0';

        printf("Process A: Received -> %s\n", buffer);
    }

    // Close the FIFOs
    fclose(fp_write);
    fclose(fp_read);

    printf("Process A (%d): Exiting.\n", pid);
    return 0;
}
