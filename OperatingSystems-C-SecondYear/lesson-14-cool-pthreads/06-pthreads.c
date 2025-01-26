// File: thread_safe_counter.c
// Compile with: gcc -Wall -pthread -o thread_safe_counter thread_safe_counter.c

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 5
#define INCREMENTS_PER_THREAD 1000000

// Shared counter
long long counter = 0;

// Mutex to protect the counter
pthread_mutex_t counter_mutex;

void* increment_counter(void* arg) {
    for (int i = 0; i < INCREMENTS_PER_THREAD; i++) {
        // Lock the mutex before modifying the counter
        pthread_mutex_lock(&counter_mutex);
        
        // Critical section
        counter++;
        
        // Unlock the mutex after modification
        pthread_mutex_unlock(&counter_mutex);
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NUM_THREADS];
    int rc;

    // Initialize the mutex
    if (pthread_mutex_init(&counter_mutex, NULL) != 0) {
        perror("Mutex initialization failed");
        exit(EXIT_FAILURE);
    }

    // Create threads
    for (long t = 0; t < NUM_THREADS; t++) {
        rc = pthread_create(&threads[t], NULL, increment_counter, NULL);
        if (rc) {
            fprintf(stderr, "Error: Unable to create thread %ld, return code %d\n", t, rc);
            exit(EXIT_FAILURE);
        }
    }

    // Wait for all threads to finish
    for (int t = 0; t < NUM_THREADS; t++) {
        pthread_join(threads[t], NULL);
    }

    // Destroy the mutex
    pthread_mutex_destroy(&counter_mutex);

    // Expected counter value: NUM_THREADS * INCREMENTS_PER_THREAD
    printf("Final counter value: %lld (Expected: %d)\n", counter, NUM_THREADS * INCREMENTS_PER_THREAD);
    return 0;
}
