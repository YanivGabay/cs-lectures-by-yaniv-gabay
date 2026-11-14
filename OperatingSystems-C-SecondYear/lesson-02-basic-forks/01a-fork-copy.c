/*
 * 01a-fork-copy.c — After fork() the child has a COPY of the parent's memory
 *
 * Key concepts: separate address spaces, same virtual address, wait()
 * Compile: gcc -o fork_copy 01a-fork-copy.c
 * Run:     ./fork_copy
 *
 * The child changes x. The parent's x does not change — even though both print
 * the same address &x. Each process has its own (virtual) memory.
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void)
{
    int x = 10;
    printf("[Before fork] x = %d at address %p\n", x, (void *)&x);
    fflush(stdout);

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        return EXIT_FAILURE;
    }

    if (pid == 0) {                       // child
        x += 5;
        printf("[Child  %d] changed x to %d   (address %p)\n", getpid(), x, (void *)&x);
        exit(EXIT_SUCCESS);
    }

    wait(NULL);                           // parent: let the child finish first
    printf("[Parent %d] x is still %d    (address %p)\n", getpid(), x, (void *)&x);
    printf("Same address, different values: two separate copies of memory.\n");
    return EXIT_SUCCESS;
}
