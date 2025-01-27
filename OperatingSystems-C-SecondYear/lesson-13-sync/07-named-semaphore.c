// File: pthread_named_semaphore_example.c
// Compile with: gcc -Wall -pthread pthread_named_semaphore_example.c -o pthread_named_semaphore_example

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>   // For semaphore functions
#include <fcntl.h>       // For O_CREAT, O_EXCL
#include <sys/stat.h>    // For file mode constants
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
        snprintf(message, sizeof(message), "Thread %d is in critical section (iteration %d)", id, i);
        printf("%s\n", message);
        sleep(1);  // Simulate work in the critical section.
        
        // Release (increment) the semaphore.
        if (sem_post(sem) != 0) {
            perror("sem_post failed");
            pthread_exit(NULL);
        }
        
        // Simulate some non-critical work.
        sleep(1);
    }
    pthread_exit(NULL);
}

int main(void) {
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
    
    printf("Main: Named semaphore '%s' created successfully.\n", SEM_NAME);
    
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
    
    printf("Main: All threads finished. Semaphore unlinked.\n");
    return EXIT_SUCCESS;
}
