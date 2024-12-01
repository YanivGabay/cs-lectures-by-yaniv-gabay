#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

// Handler function for SIGINT
void sigint_handler(int signum) {
    printf("\nCaught SIGINT (signal number %d). Exiting gracefully.\n", signum);
    exit(0);
}

int main() {
    struct sigaction sa;

    // Set up the sigaction struct
    sa.sa_handler = sigint_handler;    // Assign handler function
    sa.sa_flags = 0;                    // No special flags
    sigemptyset(&sa.sa_mask);           // Don't block any signals during handler

    // Set up the handler for SIGINT
    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("Error: sigaction failed");
        exit(EXIT_FAILURE);
    }

    printf("Process PID: %d\n", getpid());
    printf("Press Ctrl+C to trigger SIGINT.\n");

    // Infinite loop to keep the program running
    while (1) {
        printf("Running... Press Ctrl+C to exit.\n");
        sleep(2);
    }

    return 0;
}
