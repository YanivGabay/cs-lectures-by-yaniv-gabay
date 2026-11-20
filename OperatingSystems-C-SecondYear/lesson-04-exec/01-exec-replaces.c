/*
 * 01-exec-replaces.c — exec does not start a new process: it REPLACES the program in this one
 *
 * Key concepts: same PID before and after exec, exec never returns on success
 * Compile: gcc -o exec_replaces 01-exec-replaces.c
 * Run:     ./exec_replaces
 *
 * fork() makes a new process. exec() keeps the process (same PID, same open files)
 * but throws away its code and memory and loads another program instead.
 * So the printf after a successful exec can never run: that code is gone.
 */
#include <stdio.h>
#include <unistd.h>

int main(void)
{
    printf("[Before exec] I am PID %d, running 01-exec-replaces.c\n", getpid());
    fflush(stdout);                       // the buffer is thrown away by exec — flush first!

    // sh prints its own PID ($$): the same number, because it is the same process
    execl("/bin/sh", "sh", "-c", "echo \"[After exec]  I am PID $$, now running sh\"", (char *)NULL);

    printf("This line runs only if exec FAILED.\n");
    return 1;
}
