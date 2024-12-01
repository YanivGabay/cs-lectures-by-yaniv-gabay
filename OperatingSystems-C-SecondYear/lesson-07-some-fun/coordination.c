#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>

// Global variables to store child PIDs
pid_t child1_pid = 0;
pid_t child2_pid = 0;

// Function to set up signal handlers using sigaction
void setup_sigaction(int signum, void (*handler)(int)) {
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = handler;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);

    if (sigaction(signum, &sa, NULL) == -1) {
        perror("Failed to set up sigaction");
        exit(EXIT_FAILURE);
    }
}

// Signal handler for SIGUSR1 (from Child 1)
void handle_sigusr1(int signum) {
    printf("Parent: Received SIGUSR1 from Child 1. Task acknowledged.\n");
    // Parent sends a signal to Child 2 to start its task
    if (kill(child2_pid, SIGUSR2) == -1) {
        perror("Parent failed to send SIGUSR2 to Child 2");
    } else {
        printf("Parent: Sent SIGUSR2 to Child 2 to start its task.\n");
    }
}

// Signal handler for SIGUSR2 (from Child 2)
void handle_sigusr2(int signum) {
    printf("Parent: Received SIGUSR2 from Child 2. Task completed.\n");
    printf("Parent: All tasks are completed. Terminating children.\n");
    kill(child1_pid, SIGTERM);
    kill(child2_pid, SIGTERM);
}

int main() {
    // Set up the signal handlers in the parent using sigaction
    setup_sigaction(SIGUSR1, handle_sigusr1);
    setup_sigaction(SIGUSR2, handle_sigusr2);

    printf("Parent PID: %d\n", getpid());

    // Fork Child 1
    child1_pid = fork();
    if (child1_pid < 0) {
        perror("Failed to fork Child 1");
        exit(EXIT_FAILURE);
    }

    if (child1_pid == 0) {
        // In Child 1
        printf("Child 1 PID: %d started. Performing task...\n", getpid());
        sleep(2); // Simulate some work
        printf("Child 1 PID: %d completed task. Sending SIGUSR1 to Parent.\n", getpid());
        kill(getppid(), SIGUSR1);
        pause(); // Wait indefinitely for signals
        exit(0);
    }

    // Fork Child 2
    child2_pid = fork();
    if (child2_pid < 0) {
        perror("Failed to fork Child 2");
        kill(child1_pid, SIGTERM); // Terminate Child 1 before exiting
        exit(EXIT_FAILURE);
    }

    if (child2_pid == 0) {
        // In Child 2
        printf("Child 2 PID: %d started. Waiting for signal from Parent to begin task...\n", getpid());
        setup_sigaction(SIGUSR2, SIG_DFL); // Default handler for SIGUSR2 to terminate process
        pause(); // Wait for SIGUSR2 from Parent
        printf("Child 2 PID: %d received signal and completed task. Sending SIGUSR2 to Parent.\n", getpid());
        kill(getppid(), SIGUSR2);
        exit(0);
    }

    // Parent process waits for signals from children
    printf("Parent: Waiting for Child 1 to complete its task...\n");
    while (1) {
        pause(); // Wait for signals indefinitely
    }

    //wait on all the children
    while (wait(NULL) > 0);

    return 0;
}
