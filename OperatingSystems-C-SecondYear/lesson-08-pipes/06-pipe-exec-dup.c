#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    int pipefd[2];
    pid_t pid_producer, pid_consumer;

    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    // Fork producer process
    pid_producer = fork();
    if (pid_producer == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid_producer == 0) { // Producer Child
        // Redirect stdout to pipe's write end
        if (dup2(pipefd[1], STDOUT_FILENO) == -1) {
            perror("dup2");
            exit(EXIT_FAILURE);
        }
        close(pipefd[0]); // Close unused read end
        close(pipefd[1]); // Close original write end

        // Execute producer program
        execlp("./producer", "./producer", (char *)NULL);
        perror("execlp producer");
        exit(EXIT_FAILURE);
    }

    // Fork consumer process
    pid_consumer = fork();
    if (pid_consumer == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid_consumer == 0) { // Consumer Child
        // Redirect stdin to pipe's read end
        if (dup2(pipefd[0], STDIN_FILENO) == -1) {
            perror("dup2");
            exit(EXIT_FAILURE);
        }
        close(pipefd[1]); // Close unused write end
        close(pipefd[0]); // Close original read end

        // Execute consumer program
        execlp("./consumer", "./consumer", (char *)NULL);
        perror("execlp consumer");
        exit(EXIT_FAILURE);
    }

    // Parent process closes both ends of the pipe
    close(pipefd[0]);
    close(pipefd[1]);

    // Wait for both children to finish
    waitpid(pid_producer, NULL, 0);
    waitpid(pid_consumer, NULL, 0);

    return 0;
}
