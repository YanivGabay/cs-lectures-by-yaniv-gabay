#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>



// this program shows how the sa_mask works
// basicly its blocks the signal that is in the mask
// while the handler is running
// so in this program, we want first to send the sigusr1 signal
// and then the sigusr2 signal (we have 5 secs window to send the sigusr2 signal)
// but the sigusr2 signal will be blocked until the handler of sigusr1 is done

// Handler for SIGUSR1
void handle_sigusr1(int signum) {
    printf("\nHandling SIGUSR1 (signal number %d). SIGUSR2 is blocked during this handler.\n", signum);
    // Simulate long processing time
    sleep(5);
    printf("Finished handling SIGUSR1.\n");
}

// Handler for SIGUSR2
void handle_sigusr2(int signum) {
    printf("\nHandling SIGUSR2 (signal number %d).\n", signum);
}

int main() {
    struct sigaction sa_usr1, sa_usr2;

    // Setup handler for SIGUSR1 with SIGUSR2 blocked during handler
    sa_usr1.sa_handler = handle_sigusr1;
    sa_usr1.sa_flags = 0;
    sigemptyset(&sa_usr1.sa_mask);          // Initialize sa_mask
    sigaddset(&sa_usr1.sa_mask, SIGUSR2);   // Block SIGUSR2 during SIGUSR1 handler

    if (sigaction(SIGUSR1, &sa_usr1, NULL) == -1) {
        perror("Error: sigaction for SIGUSR1 failed");
        exit(EXIT_FAILURE);
    }

    // Setup handler for SIGUSR2
    sa_usr2.sa_handler = handle_sigusr2;
    sa_usr2.sa_flags = 0;
    sigemptyset(&sa_usr2.sa_mask);          // No additional signals blocked

    if (sigaction(SIGUSR2, &sa_usr2, NULL) == -1) {
        perror("Error: sigaction for SIGUSR2 failed");
        exit(EXIT_FAILURE);
    }

    printf("Process PID: %d\n", getpid());
    printf("Send SIGUSR1 and than (in a 5 secs window) SIGUSR2 signals to see blocking in action.\n");

    // Infinite loop to keep the program running
    while (1) {
        printf("Program idle... Waiting for signals.\n");
        sleep(2);
    }

    return 0;
}
