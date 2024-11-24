#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
//#include <unistd.h>

void signal_handler(int sig) {
    printf("\nSignal %d received (using signal). You pressed Ctrl+C!\n", sig);
}

int main() {
    // Setting up the signal handler using signal()
    signal(SIGINT, signal_handler);

    int number;
    printf("Enter a number (use Ctrl+C to send SIGINT and observe behavior): ");

    // scanf will be interrupted by SIGINT, but will resume after the signal handler returns
    if (scanf("%d", &number) == 1) {
        printf("You entered: %d\n", number);
    } else {
        printf("\nFailed to read a number.\n");
    }
    printf("Program continues after scanf.\n");

    return 0;
}
