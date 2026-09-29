/*
 * basic_exec.c — exec family — execl, execlp, execv, execvp variants
 *
 * Key concepts: exec replaces process image, 'l' vs 'v', 'p' for PATH search
 * Compile: gcc -o exec basic_exec.c
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
    printf("  exec family — execl, execlp, execv\n");
    printf("========================================\n\n");
    const int EXAMPLE = 1;

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(1);
    }

    if (pid == 0)
    {
        printf("[Child  PID %d] About to call exec — this process image will be REPLACED.\n\n", getpid());
        int status;
        switch (EXAMPLE)
        {
        case 1:
            printf("[Child] Using execl(\"/bin/ls\", \"ls\", \"-l\", NULL)\n");
            printf("        'l' = list arguments, no 'p' = full path required\n\n");
            status = execl("/bin/ls" ,"ls", "-l", NULL);
            break;
        case 2:
            printf("[Child] Using execv(\"/bin/ls\", args)\n");
            printf("        'v' = vector (char* array), no 'p' = full path required\n\n");
            char *args[] = {"ls", "-l", NULL};
            status = execv("/bin/ls", args);
            break;

        default:
            break;
        }
        if (status == -1) {
            perror("[Child] exec failed");
            exit(1);
        }

    }
    else {
        printf("[Parent PID %d] Created child PID %d. Waiting...\n", getpid(), pid);
        wait(NULL);
        printf("[Parent PID %d] Child finished. exec replaced its entire process image.\n\n", getpid());
        printf("========================================\n");
    }

    return 0;
}