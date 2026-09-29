/*
 * 03-basic-fork-thefix.c — Correct way to fork in a loop
 *
 * Demonstrates: Fixing the fork-in-a-loop bug from 02-basic-fork-mistake.c
 * Key concepts: Child must exit() after its work, parent must wait() for each child
 * Compile: gcc -o fork_fix 03-basic-fork-thefix.c
 * Run:     ./fork_fix
 *
 * Two fixes applied:
 *   1. Child calls exit(0) so it doesn't continue the loop
 *   2. Parent calls wait(NULL) to reap each child before forking the next
 */

#include <stdio.h>
#include <stdlib.h>    // for exit(), EXIT_FAILURE
#include <unistd.h>    // for fork(), getpid()
#include <sys/wait.h>  // for wait()

void check_valid_process(pid_t status);


int main() {

    printf("\n");
    printf("══════════════════════════════════════\n");
    printf("  Correct way to fork in a loop\n");
    printf("══════════════════════════════════════\n\n");
    printf("  Fixes: child calls exit(), parent\n");
    printf("  calls wait() before next fork.\n");

    const int RUNS = 5;
    pid_t status;
    for (int i = 0; i < RUNS; i++) {
        printf("\n--- Run %d ---\n", i + 1);
        
        pid_t pid = fork();  // fork() creates a new process

        if (pid < 0) {
            // Fork failed
            perror("fork failed");
            return 1;
        } else if (pid == 0) {
            // Child process
            printf("[Child  PID %d] Hello from child (run %d)! I will exit(0) now.\n", getpid(), i + 1);
            exit(0);  // FIX #1: Child exits here — won't continue the for-loop
        } else {
            // Parent process
            printf("[Parent PID %d] Created child PID %d. Waiting for it...\n", getpid(), pid);
            // FIX #2: Parent waits for child to finish before next iteration
            // wait(NULL) returns child PID on success, -1 on failure
            status = wait(NULL);
            if (status < 0) {
                perror("Failed to wait for child process");
                exit(EXIT_FAILURE);
            }
            else {
                printf("[Parent PID %d] wait() returned %d — child has been reaped.\n", getpid(), status);
            }
        }
    }

    return 0;
}

// Example function to check if the process is valid; exits if not
void check_valid_process(pid_t status) {
    if (status < 0) {
        perror("Failed to create process");
        exit(EXIT_FAILURE);
    }
}
