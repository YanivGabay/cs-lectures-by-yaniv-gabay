/*
 * calculator_exec.c — Parent program that fork+exec's the calculator
 *
 * Key concepts: fork+exec pattern, parent passes args to child via exec
 * Compile: gcc -o calc_exec calculator_exec.c
 * Run:     ./prog
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    printf("\n");
    printf("========================================\n");
    printf("  fork() + exec() — Launch Calculator\n");
    printf("========================================\n\n");
    printf("[Parent PID %d] Will fork, then exec the calculator program.\n\n", getpid());
    const int EXAMPLE = 1;

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork failed");
        exit(1);
    }

    if (pid == 0) {
        printf("[Child  PID %d] Replacing myself with ./calculator via exec...\n", getpid());
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
        printf("[Parent PID %d] Waiting for child PID %d to complete...\n", getpid(), pid);
        wait(NULL);
        printf("[Parent PID %d] Child finished. The calculator ran in a separate process.\n\n", getpid());
        printf("========================================\n");
    }

    return 0;
}
