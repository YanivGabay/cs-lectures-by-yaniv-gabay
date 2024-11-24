#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
//#include <unistd.h>

// Signal handler function for SIGINT
void sigint_handler(int sig) {
    printf("\nSignal %d received (using sigaction). You pressed Ctrl+C!\n", sig);
}

int main() {
    struct sigaction sa;

    // Set up sigaction structure
    sa.sa_handler = sigint_handler;    // Assign handler function
    sa.sa_flags = 0;                   // No special flags (default behavior)
    sigemptyset(&sa.sa_mask);          // Block no additional signals during handler execution

    // Setting up the signal handler using sigaction()
    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("Error: sigaction failed");
        exit(EXIT_FAILURE);
    }

    int number;
    printf("Enter a number (use Ctrl+C to send SIGINT and observe behavior): ");

    // scanf will be interrupted by SIGINT, and after the handler, the program proceeds to the next statement
    if (scanf("%d", &number) == 1) {
        printf("You entered: %d\n", number);
    } else {
        printf("\nFailed to read a number.\n");
    }
    printf("Program continues after scanf.\n");

    return 0;
}
