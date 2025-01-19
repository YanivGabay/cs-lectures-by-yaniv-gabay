// File: mutex_counter.c
// Compile with: gcc -Wall -pthread mutex_counter.c -o mutex_counter

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

// Global shared counter
int counter = 0;

// Mutex to protect the counter
pthread_mutex_t counter_mutex = PTHREAD_MUTEX_INITIALIZER;

// Each thread will increment the counter a fixed number of times.
void* increment_counter(void *arg) {
    int id = *((int *) arg);
    for (int i = 0; i < 100; i++) {
        // Lock the mutex before accessing the shared counter.
        pthread_mutex_lock(&counter_mutex);
        counter++;  // Critical section: safe update
        printf("Thread %d incremented counter to %d\n", id, counter);
        // Unlock the mutex immediately after finishing the update.
        pthread_mutex_unlock(&counter_mutex);
        usleep(10000); // Sleep for 10 milliseconds to increase interleaving.
    }
    pthread_exit(NULL);
}

int main() {
    const int NUM_THREADS = 5;
    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS];
    int status;

    // Seed the random number generator (not essential for mutex, but good practice).
    srand((unsigned)time(NULL));

    // Create multiple threads.
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_ids[i] = i + 1;
        status = pthread_create(&threads[i], NULL, increment_counter, (void *) &thread_ids[i]);
        if (status != 0) {
            fprintf(stderr, "pthread_create failed for thread %d\n", i + 1);
            exit(EXIT_FAILURE);
        }
    }

    // Wait for all threads to finish.
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // Print the final counter value.
    printf("Final counter value (expected %d): %d\n", NUM_THREADS * 100, counter);

    // Destroy the mutex (good practice when done).
    pthread_mutex_destroy(&counter_mutex);
    return EXIT_SUCCESS;
}
