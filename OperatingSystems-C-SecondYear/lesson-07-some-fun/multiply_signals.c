#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>

// Number of child processes
#define NUM_CHILDREN 4

// Function prototypes for signal handlers
void handle_sigint(int sig);
void handle_sigusr1(int sig);
void handle_sigusr2(int sig);
void handle_sigterm(int sig);

int main() {
    pid_t pid;
    struct sigaction sa_int, sa_usr1, sa_usr2, sa_term;

    // Setting up SIGINT handler
    sa_int.sa_handler = handle_sigint;
    sa_int.sa_flags = 0;
    sigemptyset(&sa_int.sa_mask);
    if (sigaction(SIGINT, &sa_int, NULL) == -1) {
        perror("Error: cannot handle SIGINT");
        exit(EXIT_FAILURE);
    }

    // Setting up SIGUSR1 handler
    sa_usr1.sa_handler = handle_sigusr1;
    sa_usr1.sa_flags = 0;
    sigemptyset(&sa_usr1.sa_mask);
    if (sigaction(SIGUSR1, &sa_usr1, NULL) == -1) {
        perror("Error: cannot handle SIGUSR1");
        exit(EXIT_FAILURE);
    }

    // Setting up SIGUSR2 handler
    sa_usr2.sa_handler = handle_sigusr2;
    sa_usr2.sa_flags = 0;
    sigemptyset(&sa_usr2.sa_mask);
    if (sigaction(SIGUSR2, &sa_usr2, NULL) == -1) {
        perror("Error: cannot handle SIGUSR2");
        exit(EXIT_FAILURE);
    }

    // Setting up SIGTERM handler
    sa_term.sa_handler = handle_sigterm;
    sa_term.sa_flags = 0;
    sigemptyset(&sa_term.sa_mask);
    if (sigaction(SIGTERM, &sa_term, NULL) == -1) {
        perror("Error: cannot handle SIGTERM");
        exit(EXIT_FAILURE);
    }

    printf("Parent PID: %d\n", getpid());
    printf("Setting up %d child processes to send signals.\n", NUM_CHILDREN);

    // Array of signals to be sent by children
    int signals[NUM_CHILDREN] = {SIGINT, SIGUSR1, SIGUSR2, SIGTERM};

    // Forking child processes
    for (int i = 0; i < NUM_CHILDREN; i++) {
        pid = fork();
        if (pid < 0) {
            perror("Fork failed");
            exit(EXIT_FAILURE);
        }

        if (pid == 0) {
            // Child process
            sleep(2 * (i + 1)); // Stagger the signals
            printf("Child %d sending signal %d to parent.\n", getpid(), signals[i]);
            if (kill(getppid(), signals[i]) == -1) {
                perror("Failed to send signal");
                exit(EXIT_FAILURE);
            }
            exit(EXIT_SUCCESS);
        }
    }

    // Parent process waits for signals indefinitely
    printf("Parent is waiting for signals. Press Ctrl+C to send SIGINT manually.\n");

    while (1) {
        pause(); // Wait for signals
    }

    //

    // This point is never reached
    return 0;
}

// Handler for SIGINT
void handle_sigint(int sig) {
    printf("\nReceived SIGINT (Interrupt Signal). Handling gracefully.\n");
    // Additional actions can be performed here
}

// Handler for SIGUSR1
void handle_sigusr1(int sig) {
    printf("Received SIGUSR1 (User-Defined Signal 1). Performing action 1.\n");
    // Additional actions can be performed here
}

// Handler for SIGUSR2
void handle_sigusr2(int sig) {
    printf("Received SIGUSR2 (User-Defined Signal 2). Performing action 2.\n");
    // Additional actions can be performed here
}

// Handler for SIGTERM
void handle_sigterm(int sig) {
    printf("Received SIGTERM (Termination Signal). Cleaning up and exiting.\n");
    // Perform any necessary cleanup here
    

    //before exiting, we need to wait for all the children to finish
    //we can do that by using the wait function

    while (wait(NULL) > 0); // Wait for all child processes to finish
    exit(EXIT_SUCCESS);
}
