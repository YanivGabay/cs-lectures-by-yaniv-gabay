#include <stdio.h>
#include <stdlib.h> // for exit()
#include <unistd.h> // for fork()
#include <sys/wait.h> // for wait()


// Function to check if the process creation was valid
void check_valid_process(pid_t status);


//this is the "solution" to the problem
/*
the fixes:
1. we added a wait() function to the parent process, so it will wait for the child process to finish
2. we added an exit() function to the child process, so it will not continue to fork

*/


int main() {
    const int RUNS = 5;  // Number of times to run the fork process
    pid_t status;
    for (int i = 0; i < RUNS; i++) {
        printf("\n--- Run %d ---\n", i + 1);
        
        pid_t pid = fork();  // fork() creates a new process

        if (pid < 0) {
            // Fork failed
            perror("fork failed");
            return 1;
        } else if (pid == 0) {
            // Child process
            printf("This is the child process. PID: %d\n", getpid());
            exit(0);  // Exit the child process to prevent further forking
        } else {
            // Parent process
            printf("This is the parent process. PID: %d, Child PID: %d\n", getpid(), pid);
            //from man 7 
            // https://man7.org/linux/man-pages/man2/waitpid.2.html
            //wait(): on success, returns the process ID of the terminated
            //child; on failure, -1 is returned.
            status = wait(NULL);  // Wait for the child process to complete
            if (status < 0) {
                perror("Failed to wait for child process");
                exit(EXIT_FAILURE);
            }
            else {
                printf("status value of exited procees: %d\n" ,status)
            }
        }
    }

    return 0;
}

// Example function to check if the process is valid; exits if not
void check_valid_process(pid_t status) {
    if (status < 0) {
        perror("Failed to create process");
        exit(EXIT_FAILURE);
    }
}
