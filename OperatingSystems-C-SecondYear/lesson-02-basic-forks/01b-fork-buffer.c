/*
 * 01b-fork-buffer.c — Why does this line print twice? fork() copies the output buffer too
 *
 * Key concepts: stdio buffering, fflush(), line-buffered terminal vs fully-buffered file
 * Compile: gcc -o fork_buffer 01b-fork-buffer.c
 * Run:     ./fork_buffer                 (to the terminal)
 *          ./fork_buffer > out.txt       (to a file — then open out.txt in the Files panel)
 *
 * printf() does not write immediately: text waits in a buffer inside the process.
 * fork() copies the whole process — buffer included — so both copies print it later.
 * A terminal flushes at every '\n'; a file only when the buffer is full or at exit,
 * so redirected to a file even lines WITH '\n' come out twice.
 * (Here the very first line still appears once: this sandbox's C library, musl, decides
 *  "terminal or file?" at its first write. On a typical laptop with glibc, it doubles too.)
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void)
{
    printf("1. A line ending with newline\n");
    printf("2. Another line ending with newline\n");
    printf("3. No newline yet... ");      // still sitting in the buffer

    pid_t pid = fork();                   // the buffer is copied into the child
    if (pid == -1) {
        perror("fork");
        return EXIT_FAILURE;
    }

    if (pid == 0) {
        printf("[child]\n");
        exit(EXIT_SUCCESS);               // exit() flushes the child's copy of the buffer
    }
    wait(NULL);
    printf("[parent]\n");

    // The fix: call fflush(stdout) right before fork()
    return EXIT_SUCCESS;
}
