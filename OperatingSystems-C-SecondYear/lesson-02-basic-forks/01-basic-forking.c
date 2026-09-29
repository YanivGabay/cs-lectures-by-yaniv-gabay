/*
 * 01-basic-forking.c — Your first fork() example
 *
 * Demonstrates: Creating a child process with fork()
 * Key concepts: fork() return values, PID, parent vs child execution paths
 * Compile: gcc -o basic_fork 01-basic-forking.c
 * Run:     ./basic_fork
 *
 * NOTE: This code runs on Linux/macOS only — Windows does not support fork().
 */

#include <stdio.h>
#include <stdlib.h>    // for exit(), EXIT_FAILURE
#include <unistd.h>    // for fork(), getpid()
#include <sys/wait.h>  // for wait()

/*
 * fork() creates a new process by duplicating the calling process.
 * After fork(), TWO processes run the same code from the same point.
 *
 * Return values:
 *   Negative : fork failed (system out of resources)
 *   Zero (0) : you are in the CHILD process
 *   Positive : you are in the PARENT process, value = child's PID
 *
 * Tip: man pages are your best friend — https://man7.org/linux/man-pages/
 */


void check_valid_process(pid_t status);

int main()
{

    printf("\n");
    printf("========================================\n");
    printf("  Your First fork() — Parent vs Child\n");
    printf("========================================\n\n");

    pid_t pid = fork(); // fork() creates a new process

    if (pid < 0)
    {
        // I RECOMMEND TO CREATE A FUNCTION FOR ALL FUTURE ERROR HANDLING
        // Fork failed
        perror("fork failed");
        return 1;
    }
    else if (pid == 0)
    {
        // Child process
        printf("[Child]  I am the child process!  My PID: %d, my parent's PID: %d\n", getpid(), getppid());
        return 0;
    }
    else
    {
        // Parent process
        printf("[Parent] I am the parent process! My PID: %d, fork() returned child PID: %d\n", getpid(), pid);
        // NOTE: No wait() here on purpose! The parent exits without reaping the child.
        // This creates a zombie process — the next example (01.5-forking-zombies.c) explains why.
        // The fix is shown in 03-basic-fork-thefix.c.
        return 0;
    }

    // This code is unreachable — both branches return above.
    // The wait() call below never executes, which is the point:
    // students should notice the missing wait() and understand its consequences.
    wait(NULL);
    return 0;
}

// example check if the process is valid,else we exit the program
void check_valid_process(pid_t status)
{
    if (status < 0)
    {
        perror("Process creation failed");
        exit(EXIT_FAILURE);
    }
}