/*
 * 06-pipe-exec-dup.c — Connecting two programs with pipe + dup2 + exec
 *
 * Demonstrates: The dup2() pattern for redirecting stdin/stdout through a pipe
 * Key concepts: dup2(), execlp(), producer-consumer via pipe, fd cleanup
 * Compile: gcc -o pipe_exec 06-pipe-exec-dup.c
 *          (also compile producer.c and consumer.c from 02-pipe-operator-with-programs/)
 * Run:     ./pipe_exec
 *
 * This implements the equivalent of: ./producer | ./consumer
 * but programmatically using fork + pipe + dup2 + exec.
 */

#include <unistd.h>    // for pipe(), fork(), dup2(), execlp(), close()
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h> // for pid_t
#include <sys/wait.h>  // for waitpid()

/*
File Descriptor Duplication:

The dup2(pipefd[1], STDOUT_FILENO) in the producer
 process duplicates the write end of the pipe
 (pipefd[1]) into the standard output file descriptor
  (STDOUT_FILENO).
  
Similarly, dup2(pipefd[0], STDIN_FILENO) in the 
consumer process duplicates the read end of the pipe
 (pipefd[0]) into the standard input file descriptor 
 (STDIN_FILENO).

*/

/*

 pipe is created using pipe(pipefd).

pipefd[0]: Read end of the pipe.
pipefd[1]: Write end of the pipe.
The program forks two child processes:

The producer writes to the pipe.
The consumer reads from the pipe.
The producer and consumer are executed as separate programs using execlp().

The parent process handles cleanup and waits for both children to finish.
*/

/*
dup2(pipefd[1], STDOUT_FILENO) duplicates the write end of the pipe into the standard output (STDOUT_FILENO).
This means anything the producer writes to stdout will go into the pipe.
Cleanup:

The producer doesn't need pipefd[0] (read end), so it closes it.
The original pipefd[1] is also closed because dup2() has already duplicated it.

*/

/*
Redirection:

dup2(pipefd[0], STDIN_FILENO) duplicates the read end of the pipe into the standard input (STDIN_FILENO).
This means the consumer will read its input from the pipe instead of the terminal.
Cleanup:

The consumer doesn't need pipefd[1] (write end), so it closes it.
The original pipefd[0] is also closed after dup2().

*/


int main() {
    printf("\n");
    printf("══════════════════════════════════════\n");
    printf("  pipe + dup2 + exec\n");
    printf("  Programmatic: ./producer | ./consumer\n");
    printf("══════════════════════════════════════\n\n");
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

    printf("[Parent PID %d] pipe() created: read_fd=%d, write_fd=%d\n", getpid(), pipefd[0], pipefd[1]);

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

    printf("[Parent] Forked producer (PID %d) and consumer (PID %d).\n", pid_producer, pid_consumer);
    printf("[Parent] dup2: producer stdout → pipe write end, consumer stdin → pipe read end.\n");
    printf("[Parent] Closing pipe fds and waiting for children...\n\n");

    close(pipefd[0]);
    close(pipefd[1]);

    waitpid(pid_producer, NULL, 0);
    waitpid(pid_consumer, NULL, 0);
    printf("[Parent] Both children finished.\n");

    return 0;
}
