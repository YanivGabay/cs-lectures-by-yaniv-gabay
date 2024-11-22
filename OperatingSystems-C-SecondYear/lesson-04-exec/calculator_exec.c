#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    const int EXAMPLE = 1; // Change this to test different exec variants

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork failed");
        exit(1);
    }

    if (pid == 0) {
        // Child process
        printf("Child process executing the calculator program...\n");
        int status;

        switch (EXAMPLE) {
            case 1: {
                // Using execl: Pass arguments as separate strings
                status = execl("./calculator", "calculator", "5", "+", "3", NULL);
                break;
            }
            case 2: {
                // Using execv: Pass arguments as a char array
                char *args[] = {"calculator", "8", "*", "4", NULL};
                status = execv("./calculator", args);
                break;
            }
            default:
                fprintf(stderr, "Invalid EXAMPLE value\n");
                exit(1);
        }

        if (status == -1) {
            perror("exec failed");
            exit(1);
        }
    } else {
        // Parent process
        printf("Parent process waiting for the child to complete...\n");
        wait(NULL); // Wait for the child to finish
        printf("Parent process finished.\n");
    }

    return 0;
}
