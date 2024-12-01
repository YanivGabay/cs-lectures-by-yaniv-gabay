#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
 #include <sys/types.h>
int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <seconds> <parent_pid>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int seconds = atoi(argv[1]);
    pid_t parent_pid = atoi(argv[2]);

    // Sleep for the specified number of seconds
    sleep(seconds);

    // Send SIGUSR1 to the parent process
    if (kill(parent_pid, SIGUSR1) == -1) {
        perror("Failed to send signal to parent");
        exit(EXIT_FAILURE);
    }

    
    printf("Alarm triggered after %d seconds.\n", seconds);

    return 0;
}
