#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>


// here we show example of two flags
// SA_RESTART and SA_RESETHAND
// SA_RESTART is used to restart the system call if it was interrupted by the signal (sleep in this case)
// SA_RESETHAND is used to reset the handler to default after the first signal

// here we want to use :
// kill -SIGUSR1 <pid>
// than again
// kill -SIGUSR1 <pid>

// Handler for SIGUSR1
void handle_sigusr1(int signum) {
    printf("\nCaught SIGUSR1 (signal number %d). This handler will reset to default after this.\n", signum);
}

int main() {
    struct sigaction sa;

    // Setup handler for SIGUSR1 with SA_RESTART and SA_RESETHAND flags
    sa.sa_handler = handle_sigusr1;
    sa.sa_flags = SA_RESTART | SA_RESETHAND; // Restart interrupted syscalls and reset handler
    sigemptyset(&sa.sa_mask);                  // No additional signals blocked

    if (sigaction(SIGUSR1, &sa, NULL) == -1) {
        perror("Error: sigaction for SIGUSR1 failed");
        exit(EXIT_FAILURE);
    }

    printf("Process PID: %d\n", getpid());
    printf("Send SIGUSR1 signals. Handler will reset after first signal.\n");

    // Infinite loop to keep the program running and handling signals
    while (1) {
        printf("Program running... Waiting for SIGUSR1.\n");
        sleep(4);
    }

    return 0;
}
