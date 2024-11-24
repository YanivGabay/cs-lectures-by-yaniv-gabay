#include <stdio.h>
#include <unistd.h>
#include <signal.h>

// Signal handler for SIGALRM
void handle_alarm(int sig) ;

int main() {
    int countdown = 5;

    // Register the signal handler
    signal(SIGALRM, handle_alarm);

    // Set the alarm for 5 seconds
    printf("Countdown timer: %d seconds. Wait for it...\n", countdown);
    alarm(countdown);

    // Keep the program running
    while (1) {
        // Do nothing, waiting for the alarm signal
    }

    return 0;
}

void handle_alarm(int sig) {
    printf("Time's up! Alarm triggered (SIGALRM).\n");
    exit(0); // Exit the program
}