#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>


// in order to exit this program 
// ctrl + c wont work
// so we use the terminal to send a signal to the program
// kill -SIGTERM <pid>


// Handler for SIGINT
void handle_sigint(int signum);

// Handler for SIGTERM
void handle_sigterm(int signum);

int main() {
    struct sigaction sa_int, sa_term;

    // Setup handler for SIGINT
    sa_int.sa_handler = handle_sigint;
    sa_int.sa_flags = 0;
    sigemptyset(&sa_int.sa_mask);

    if (sigaction(SIGINT, &sa_int, NULL) == -1) {
        perror("Error: sigaction for SIGINT failed");
        exit(EXIT_FAILURE);
    }

    // Setup handler for SIGTERM
    sa_term.sa_handler = handle_sigterm;
    sa_term.sa_flags = 0;
    sigemptyset(&sa_term.sa_mask);

    if (sigaction(SIGTERM, &sa_term, NULL) == -1) {
        perror("Error: sigaction for SIGTERM failed");
        exit(EXIT_FAILURE);
    }

    printf("Process PID: %d\n", getpid());
    printf("Press Ctrl+C to send SIGINT or use 'kill' command to send SIGTERM.\n");

    // Infinite loop to keep the program running
    while (1) {
        printf("Program running... PID: %d\n", getpid());
        sleep(3);
    }

    return 0;
}

// Functions

void handle_sigint(int signum) {
    printf("\nReceived SIGINT (signal number %d). Press Ctrl+C again to exit.\n", signum);
}

// Handler for SIGTERM
void handle_sigterm(int signum) {
    printf("\nReceived SIGTERM (signal number %d). Terminating program.\n", signum);
    exit(0);
}