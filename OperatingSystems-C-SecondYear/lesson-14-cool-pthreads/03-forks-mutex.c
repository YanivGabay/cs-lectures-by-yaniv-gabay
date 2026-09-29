/*
 * 03-forks-mutex.c — Fork + semaphore — file-based synchronization across processes
 *
 * Key concepts: Named semaphore with forked processes, sem_open, sem_wait, sem_post
 * Compile: gcc -o fork_sem 03-forks-mutex.c -lpthread
 * Run:     ./prog
 */
// File: synchronized_file_write.c
// Compile with: gcc -Wall -o synchronized_file_write synchronized_file_write.c -lrt

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>    // For O_CREAT, O_RDWR
#include <sys/stat.h> // For mode constants
#include <semaphore.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>

#define NUM_PROCESSES 5
#define SEM_NAME "/my_named_semaphore"
#define FILE_NAME "shared_output.txt"

int main()
{
    printf("\n");
    printf("══════════════════════════════════════\n");
    printf("  Fork + Semaphore — File Sync\n");
    printf("══════════════════════════════════════\n\n");
    printf("[Parent] %d child processes will write to '%s' protected by semaphore.\n\n", NUM_PROCESSES, FILE_NAME);
    pid_t pid;
    sem_t *mutex;

    // Remove any existing semaphore with the same name to prevent conflicts
    // good practice not mandatory
    int status = sem_unlink(SEM_NAME);
    if (status == -1)
    {
        if (errno != ENOENT)
        { // ENOENT: No such file or directory to if it isnt, then error
            perror("sem_unlink failed");
            exit(EXIT_FAILURE);
        }
    }

    // Create and initialize the named semaphore to 1 (binary semaphore)
    mutex = sem_open(SEM_NAME, O_CREAT, 0600, 1);
    if (mutex == SEM_FAILED)
    {
        perror("sem_open failed");
        exit(EXIT_FAILURE);
    }

    // Create the shared file or truncate if it exists to ensure a clean start
    FILE *fp = fopen(FILE_NAME, "w");
    if (fp == NULL)
    {
        perror("fopen failed");
        sem_close(mutex);
        sem_unlink(SEM_NAME);
        exit(EXIT_FAILURE);
    }
    fclose(fp);

    // Spawn child processes
    for (int i = 0; i < NUM_PROCESSES; i++)
    {
        pid = fork();
        if (pid < 0)
        {
            perror("fork failed");
            // Cleanup before exiting
            sem_close(mutex);
            sem_unlink(SEM_NAME);
            exit(EXIT_FAILURE);
        }
        else if (pid == 0)
        {
            // Child process

            // Open the named semaphore
            sem_t *child_mutex = sem_open(SEM_NAME, 0);
            if (child_mutex == SEM_FAILED)
            {
                perror("Child sem_open failed");
                exit(EXIT_FAILURE);
            }

            // Wait (lock) the semaphore before entering the critical section
            if (sem_wait(child_mutex) == -1)
            {
                perror("sem_wait failed");
                sem_close(child_mutex);
                exit(EXIT_FAILURE);
            }

            // Critical section: write to the shared file
            FILE *child_fp = fopen(FILE_NAME, "a");
            if (child_fp == NULL)
            {
                perror("Child fopen failed");
                sem_post(child_mutex);
                sem_close(child_mutex);
                exit(EXIT_FAILURE);
            }

            // Create a unique message for each child
            char message[100];
            snprintf(message, sizeof(message), "Process %d (PID %d) is writing this line.\n", i + 1, getpid());
            fprintf(child_fp, "%s", message);
            printf("[Child %d, PID %d] Acquired semaphore → wrote to file → releasing.\n", i + 1, getpid());

            fclose(child_fp);

            // Post (unlock) the semaphore after exiting the critical section
            if (sem_post(child_mutex) == -1)
            {
                perror("sem_post failed");
                sem_close(child_mutex);
                exit(EXIT_FAILURE);
            }

            // Close the semaphore in the child process
            sem_close(child_mutex);
            exit(EXIT_SUCCESS);
        }
    }

    // Parent process waits for all child processes to complete
    for (int i = 0; i < NUM_PROCESSES; i++)
    {
        wait(NULL);
    }

    // Clean up: close and unlink the semaphore
    sem_close(mutex);
    sem_unlink(SEM_NAME);

    printf("\n[Parent] All children finished. Check output: cat %s\n", FILE_NAME);
    return EXIT_SUCCESS;
}
