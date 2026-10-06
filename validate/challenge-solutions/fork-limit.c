/*
 * Challenge (Lesson 02): the fork() that fails
 *
 * This loop asks for 200 children, but the sandbox lets your user own only 64
 * processes. Right now nobody notices when fork() fails — it returns -1, the
 * program treats -1 like a child's PID and carries on.
 *
 * Your task: detect the failure, print WHY with perror("fork"), and stop the loop.
 * Done when the output shows:  fork: Resource temporarily unavailable   (that is EAGAIN)
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void)
{
    int created = 0;
    for (int i = 0; i < 200; i++) {
        pid_t pid = fork();

        if (pid == -1) { perror("fork"); break; }

        if (pid == 0) {       // child: wait a moment, then leave
            sleep(2);
            _exit(0);
        }
        created++;
    }
    while (wait(NULL) > 0)    // parent: collect every child (no zombies)
        ;
    printf("Asked for 200 children, really created %d\n", created);
    return 0;
}
