/*
 * 02-basic-fork-mistake.c — The classic fork-in-a-loop bug
 *
 * Demonstrates: What goes WRONG when you fork() inside a loop without exiting
 * Key concepts: Exponential process creation, fork bomb pattern
 * Compile: gcc -o fork_mistake 02-basic-fork-mistake.c
 * Run:     ./fork_mistake
 *
 * BUG: The child process does NOT exit after printing — it continues the loop
 *      and calls fork() again! With 5 iterations, you get ~2^5 = 32 processes,
 *      not 5. See 03-basic-fork-thefix.c for the corrected version.
 */

#include <stdio.h>
#include <stdlib.h>    // for exit(), EXIT_FAILURE
#include <unistd.h>    // for fork(), getpid()

void check_valid_process(pid_t status);
int main()
{

    printf("\n");
    printf("══════════════════════════════════════\n");
    printf("  The classic fork-in-a-loop bug\n");
    printf("══════════════════════════════════════\n\n");
    printf("  Watch: children don't exit, so they\n");
    printf("  loop again and fork MORE children!\n\n");

    for (int i = 0; i < 5; i++)
    {
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
            // BUG: Child prints but does NOT exit — it loops and forks again!
            printf("[Child  PID %d] iteration %d — BUG: I will keep looping and forking more children!\n", getpid(), i);
        }
        else
        {
            // Parent process
            printf("[Parent PID %d] iteration %d — created child PID %d\n", getpid(), i, pid);
        }
    }

  

    return 0;
}

// example check if the process is valid,else we exit the program
void check_valid_process(pid_t status)
{
    if (status < 0)
    {
        perror("Failed To open procceses");
        exit(EXIT_FAILURE);
    }
}