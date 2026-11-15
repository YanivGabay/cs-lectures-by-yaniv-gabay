/*
 * 04a-graceful-shutdown.c — The safe pattern: the handler only sets a flag
 *
 * Key concepts: volatile sig_atomic_t, keep handlers tiny, finish work and close files cleanly
 * Compile: gcc -o graceful 04a-graceful-shutdown.c
 * Run:     ./graceful        (press Ctrl+C — then open work.log in the Files panel)
 *
 * A handler can interrupt main() anywhere — even in the middle of fprintf(). Doing real work
 * inside it (printf, malloc, writing files) is unsafe. So the handler only sets a flag;
 * main() notices it, finishes the current item, writes a last line and closes the file.
 */
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

volatile sig_atomic_t stop = 0;            // the only thing the handler touches

void on_sigint(int sig)
{
    (void)sig;
    stop = 1;
}

int main(void)
{
    FILE *log = fopen("work.log", "w");
    if (log == NULL) {
        perror("work.log");
        return EXIT_FAILURE;
    }
    signal(SIGINT, on_sigint);

    printf("[PID %d] writing work items to work.log — press Ctrl+C to stop\n", getpid());
    int item = 0;
    while (!stop) {
        item++;
        fprintf(log, "work item %d done\n", item);
        fflush(log);
        printf("  item %d\n", item);
        fflush(stdout);
        usleep(500000);                    // half a second per item
    }

    fprintf(log, "--- stopped by Ctrl+C after %d items, file closed cleanly ---\n", item);
    fclose(log);
    printf("\nCtrl+C received: finished item %d, wrote the last line, closed work.log.\n", item);
    return EXIT_SUCCESS;
}
