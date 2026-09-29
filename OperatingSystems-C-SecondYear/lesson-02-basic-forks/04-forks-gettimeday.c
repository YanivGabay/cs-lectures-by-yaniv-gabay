/*
 * 04-forks-gettimeday.c — Measuring parallel execution time with fork
 *
 * Demonstrates: Using gettimeofday() to time forked processes
 * Key concepts: Both parent and child run simultaneously — total time is ~2s, not ~4s
 * Compile: gcc -o fork_time 04-forks-gettimeday.c
 * Run:     ./fork_time
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>  // for gettimeofday(), struct timeval
#include <sys/wait.h>  // for wait()
#include <unistd.h>    // for fork(), sleep()

void perform_task() {
    // Simulate a task by sleeping for 2 seconds
    sleep(2);
}

int main() {
    struct timeval start, end;
    pid_t pid;
    double elapsed_time;

    printf("\n");
    printf("========================================\n");
    printf("  Parallel Execution with fork()\n");
    printf("========================================\n");
    printf("  Each process sleeps 2 seconds.\n");
    printf("  Sequential: ~4s. Parallel: ~2s.\n\n");

    gettimeofday(&start, NULL); // Get the start time

    pid = fork(); // Create a new process

    if (pid == -1) {
        // If fork() returns -1, an error occurred
        perror("Failed to fork");
        return 1;
    } else if (pid == 0) {
        // Child process
        perform_task();
        printf("[Child  PID %d] Task completed (slept 2 seconds)\n", getpid());
    } else {
        // Parent process
        perform_task();
        printf("[Parent PID %d] Task completed (slept 2 seconds). Waiting for child...\n", getpid());

        // Wait for the child to finish
        wait(NULL);
    }

    gettimeofday(&end, NULL); // Get the end time

    // Calculate the elapsed time in microseconds, then convert to seconds
    elapsed_time = (end.tv_sec - start.tv_sec) * 1000000.0; // sec to us
    elapsed_time += (end.tv_usec - start.tv_usec); // us
    elapsed_time /= 1000000.0; // convert back to seconds

    printf("[PID %d] Total elapsed time: %.2f seconds (both tasks ran in parallel!)\n", getpid(), elapsed_time);

    return 0;
}
