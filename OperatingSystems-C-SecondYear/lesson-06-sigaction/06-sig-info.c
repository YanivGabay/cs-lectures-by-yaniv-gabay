#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

// Enhanced handler for SIGUSR1 using SA_SIGINFO

// YOU DONT NEED TO USE SA_SIGINFO in the course/assigments
// it can be helpful in some cases, but its not mandatory!!
// (also the sets are not mandatory, but whatever)

//to use this program we need to send the signal with the kill command
// kill -SIGUSR1 <pid>

void handle_sigusr1(int signum, siginfo_t *info, void *context) {
    printf("\nReceived SIGUSR1 (signal number %d) from process %d.\n", signum, info->si_pid);
    printf("More info from info struct:\n");
    printf("Signal code: %d\n", info->si_code);
    printf("Signal value: %d\n", info->si_value.sival_int);
    printf("Signal errno: %d\n", info->si_errno);
    printf("Signal address: %p\n", info->si_addr);
    // also have the clock_t si_utime and si_stime // user time and system time consumed
    //lets print those
    printf("User time consumed: %ld\n", info->si_utime);
    printf("System time consumed: %ld\n", info->si_stime);

    // You can access more information from siginfo_t if needed
}

int main() {
    struct sigaction sa;

    // Setup handler for SIGUSR1 with SA_SIGINFO flag
    sa.sa_sigaction = handle_sigusr1;
    sa.sa_flags = SA_SIGINFO; // Enable extended signal information
    sigemptyset(&sa.sa_mask); // No additional signals blocked

    if (sigaction(SIGUSR1, &sa, NULL) == -1) {
        perror("Error: sigaction for SIGUSR1 failed");
        exit(EXIT_FAILURE);
    }

    printf("Process PID: %d\n", getpid());
    printf("Send SIGUSR1 signals to see extended information.\n");

    // Infinite loop to keep the program running
    while (1) {
        printf("Program idle... Waiting for SIGUSR1.\n");
        sleep(3);
    }

    return 0;
}
