#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#define BUFFER_SIZE 1024


/*

Parent Writing All Data Before Child Reads:

The parent writes the message character by character into the pipe. Since write() is non-blocking (buffer has enough space), the parent continues writing without waiting for the child to read each character.
The child only begins to process data when it executes its read() loop after the parent's write() calls have already started.
Closing the Write End:

When the parent closes my_pipe[1], it signals EOF to the child. This EOF is not immediately "received" by the child but is interpreted as the end of the data stream when the child has read all the buffered data.
After closing the write end, the child's read() detects EOF (when read() returns 0) and exits the loop.

*/

int main() {
    int pipefd[2];
    pid_t pid;
    ssize_t bytes;
    char buffer[BUFFER_SIZE];

    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid = fork();
    if (pid == 0) { // Child: Reader
        close(pipefd[1]); // Close write end
        while ((bytes = read(pipefd[0], buffer, sizeof(buffer)-1)) > 0) {
            buffer[bytes] = '\0';
            printf("Child read: %s", buffer);
        }
        close(pipefd[0]);
        exit(EXIT_SUCCESS);
    } else { // Parent: Writer
        close(pipefd[0]); // Close read end
        const char *msg = "Hello, this is a message from the parent.\n";
        size_t len = strlen(msg);
        ssize_t total_written = 0;

        while (total_written < len) {
            bytes = write(pipefd[1], msg + total_written, len - total_written);
            if (bytes == -1) {
                perror("write");
                break;
            }
            total_written += bytes;
        }
        close(pipefd[1]);
    }

    return 0;
}
