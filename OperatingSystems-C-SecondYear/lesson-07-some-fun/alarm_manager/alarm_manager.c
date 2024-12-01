#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>

#define MAX_ALARMS 10

int alarm_count = 0;

// Signal handler for SIGUSR1
void handle_alarm(int sig) {
    alarm_count++;
    printf("\nAlarm #%d triggered!\n", alarm_count);
    printf("Set another alarm or type 'exit' to quit.\nmyalarm> ");
    fflush(stdout);
}

int main() {
    pid_t pid;
    char input[100];
    struct sigaction sa;

    // Setup signal handler
    sa.sa_handler = handle_alarm;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    if (sigaction(SIGUSR1, &sa, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    printf("Simple Alarm Manager\n");
    printf("Type the number of seconds to set an alarm or 'exit' to quit.\n");

    while (1) {
        printf("myalarm> ");

        /*
        
        fflush(stdout) is used after the printf("myalarm> ");
         statement to ensure that the prompt myalarm> is displayed to the user right away,
          without waiting for the buffer to fill up or for a newline character to be encountered.
        
        */
        fflush(stdout);

        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }

        // Remove newline character
        //why this works? because fgets reads the newline character
        // and strcspn returns the number of characters before the newline
        // so the position of the newline is the number of characters before it
        // and we replace it with a null terminator
        input[strcspn(input, "\n")] = '\0';

        if (strcmp(input, "exit") == 0) {
            printf("Exiting Alarm Manager.\n");
            break;
        }

        int seconds = atoi(input);
        if (seconds <= 0) {
            printf("Please enter a valid number of seconds.\n");
            continue;
        }

        // Fork a child process
        pid = fork();
        if (pid < 0) {
            perror("Fork failed");
            continue;
        }

        if (pid == 0) {
            // Child process: Execute the alarm handler
            char seconds_str[10];
            char parent_pid_str[10];
            snprintf(seconds_str, sizeof(seconds_str), "%d", seconds);
            snprintf(parent_pid_str, sizeof(parent_pid_str), "%d", getppid());

            execlp("./alarm_handler", "./alarm_handler", seconds_str, parent_pid_str, (char *)NULL);

            //you can also do int status = execlp("./alarm_handler", "./alarm_handler", seconds_str, parent_pid_str, (char *)NULL);
            // if execlp returned, its means it failed, so we can perror and exit
            perror("execlp failed");
            exit(EXIT_FAILURE);
        } else {
            // Parent process: Continue to accept more alarms
            printf("Alarm set for %d seconds.\n", seconds);
        }
    }

    //make sure we wait on all the children so we dont have zombies
    //even if all the children are done, we still need to wait
    while (wait(NULL) > 0);

    return 0;
}
