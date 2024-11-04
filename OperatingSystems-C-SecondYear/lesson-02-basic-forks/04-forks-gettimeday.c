#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

void perform_task() {
    // Simulate a task by sleeping for 2 seconds
    sleep(2);
}

int main() {
    struct timeval start, end;
    pid_t pid;
    double elapsed_time;

    gettimeofday(&start, NULL); // Get the start time

    pid = fork(); // Create a new process

    if (pid == -1) {
        // If fork() returns -1, an error occurred
        perror("Failed to fork");
        return 1;
    } else if (pid == 0) {
        // Child process
        perform_task();
        printf("Child process completed task\n");
    } else {
        // Parent process
        perform_task();
        printf("Parent process completed task\n");

        // Wait for the child to finish
        wait(NULL);
    }

    gettimeofday(&end, NULL); // Get the end time

    // Calculate the elapsed time in microseconds, then convert to seconds
    elapsed_time = (end.tv_sec - start.tv_sec) * 1000000.0; // sec to us
    elapsed_time += (end.tv_usec - start.tv_usec); // us
    elapsed_time /= 1000000.0; // convert back to seconds

    printf("Total time taken = %f seconds\n", elapsed_time);

    return 0;
}
