/*
 * 07-named-semaphore.c — Named semaphores for thread synchronization
 *
 * Demonstrates: POSIX named semaphores with sem_open/wait/post/close/unlink
 * Key concepts: Binary semaphore (init=1) as a mutex, critical section protection,
 *               named semaphores persist in /dev/shm and work across processes too
 * Compile: gcc -Wall -o named_sem 07-named-semaphore.c -lpthread
 * Run:     ./named_sem
 *
 * Named vs unnamed semaphores:
 *   Named:   sem_open("/name", ...) — visible system-wide, survives process death
 *   Unnamed: sem_init(&sem, ...)    — lives in memory, shared via threads or shm
 *
 * If this crashes, clean up with: rm /dev/shm/sem.my_named_semaphore
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>        // for strerror()
#include <pthread.h>       // for pthread_create/join/exit
#include <unistd.h>        // for sleep()
#include <semaphore.h>     // for sem_open/wait/post/close/unlink
#include <fcntl.h>         // for O_CREAT
#include <sys/stat.h>      // for mode constants (0666)
#include <errno.h>

#define NUM_THREADS 5
#define SEM_NAME "/my_named_semaphore"
#define NUM_ITERATIONS 3

// Global pointer for our named semaphore.
sem_t *sem;

// Thread function: each thread will wait for the semaphore, print a message, and then release the semaphore.
void* thread_func(void *arg) {
    int id = *((int *) arg);
    char message[100];

    for (int i = 0; i < NUM_ITERATIONS; i++) {
        // Wait (decrement) the semaphore.
        if (sem_wait(sem) != 0) {
            perror("sem_wait failed");
            pthread_exit(NULL);
        }
        
        // Critical Section:
        printf("[Thread %d] >>> ENTERED critical section (iteration %d/%d)\n", id, i + 1, NUM_ITERATIONS);
        sleep(1);
        printf("[Thread %d] <<< LEAVING critical section — calling sem_post\n", id);

        if (sem_post(sem) != 0) {
            perror("sem_post failed");
            pthread_exit(NULL);
        }

        sleep(1);
    }
    pthread_exit(NULL);
}

int main(void) {
    printf("\n");
    printf("========================================\n");
    printf("  Named Semaphore — Mutual Exclusion\n");
    printf("========================================\n\n");
    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS];
    int status;

    // Open (or create) a named semaphore with an initial value of 1 (binary semaphore).
    // Flags: O_CREAT to create if it doesn't exist.
    // Mode: 0666 gives read/write permission for all users.
    sem = sem_open(SEM_NAME, O_CREAT, 0666, 1);
    if (sem == SEM_FAILED) {
        perror("sem_open failed");
        exit(EXIT_FAILURE);
    }
    
    printf("[Main] Named semaphore \"%s\" created (initial value=1, acts as mutex).\n", SEM_NAME);
    
    // Create NUM_THREADS threads.
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_ids[i] = i + 1;
        status = pthread_create(&threads[i], NULL, thread_func, (void *) &thread_ids[i]);
        if (status != 0) {
            fprintf(stderr, "pthread_create failed for thread %d: %s\n", i + 1, strerror(status));
            exit(EXIT_FAILURE);
        }
    }
    
    // Wait for all threads to finish.
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    
    // Close the semaphore.
    if (sem_close(sem) != 0) {
        perror("sem_close failed");
    }
    // Unlink the semaphore so that it is removed from the system.
    if (sem_unlink(SEM_NAME) != 0) {
        perror("sem_unlink failed");
    }
    
    printf("\n[Main] All threads finished.\n");
    printf("[Main] Notice: only ONE thread was in the critical section at a time!\n");
    printf("[Main] Semaphore \"%s\" unlinked (cleaned up from /dev/shm).\n", SEM_NAME);
    return EXIT_SUCCESS;
}
