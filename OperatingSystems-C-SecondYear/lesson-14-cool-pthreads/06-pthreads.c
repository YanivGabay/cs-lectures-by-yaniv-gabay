/*
 * 06-pthreads.c — Thread-safe counter with mutex
 *
 * Key concepts: pthread_mutex_lock/unlock, shared counter, thread safety
 * Compile: gcc -o counter 06-pthreads.c -lpthread
 * Run:     ./prog
 */
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
    printf("\n");
    pthread_t threads[NUM_THREADS];
    int rc;

    // Initialize the mutex
    if (pthread_mutex_init(&counter_mutex, NULL) != 0) {
        perror("Mutex initialization failed");
        exit(EXIT_FAILURE);
    }

    printf("[Main] %d threads, each incrementing %d times = %d total.\n", NUM_THREADS, INCREMENTS_PER_THREAD, NUM_THREADS * INCREMENTS_PER_THREAD);
    printf("[Main] Using mutex for every increment (safe but slow).\n\n");

    for (long t = 0; t < NUM_THREADS; t++) {
        rc = pthread_create(&threads[t], NULL, increment_counter, NULL);
        if (rc) {
            fprintf(stderr, "[Main] ERROR: pthread_create failed (thread %ld, rc=%d)\n", t, rc);
            exit(EXIT_FAILURE);
        }
        printf("[Main] Thread %ld created\n", t);
    }

    printf("[Main] Waiting for all threads...\n");
    for (int t = 0; t < NUM_THREADS; t++) {
        pthread_join(threads[t], NULL);
    }

    pthread_mutex_destroy(&counter_mutex);

    long long expected = (long long)NUM_THREADS * INCREMENTS_PER_THREAD;
    printf("\n══════════════════════════════════════\n");
    printf("  Expected: %lld\n", expected);
    printf("  Actual:   %lld %s\n", counter, counter == expected ? "✓ correct!" : "← BUG!");
    printf("══════════════════════════════════════\n");
    return 0;
}
