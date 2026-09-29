/*
 * 08-pthread-mutex.c — Fixing the race condition with a mutex
 *
 * Demonstrates: pthread_mutex_lock/unlock to protect shared data
 * Key concepts: Mutual exclusion, critical section, PTHREAD_MUTEX_INITIALIZER
 * Compile: gcc -Wall -o mutex 08-pthread-mutex.c -lpthread
 * Run:     ./mutex
 *
 * Compare with lesson-12/04-pthread_join.c which has the SAME code but NO mutex.
 * With mutex: final counter is always exactly 500 (5 threads * 100 increments).
 * Without:    final counter is unpredictable due to race conditions.
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>   // for pthread_mutex_*, pthread_create/join
#include <unistd.h>    // for usleep()
#include <time.h>

// Shared state — accessed by all 5 threads
int counter = 0;

// Mutex protects counter from concurrent access
// PTHREAD_MUTEX_INITIALIZER: static initialization (no pthread_mutex_init needed)
pthread_mutex_t counter_mutex = PTHREAD_MUTEX_INITIALIZER;

// Each thread will increment the counter a fixed number of times.
void* increment_counter(void *arg) {
    int id = *((int *) arg);
    for (int i = 0; i < 100; i++) {
        // Lock the mutex before accessing the shared counter.
        pthread_mutex_lock(&counter_mutex);
        counter++;  // Critical section: safe update
        printf("[Thread %d] counter++ → %d (mutex held — safe!)\n", id, counter);
        // Unlock the mutex immediately after finishing the update.
        pthread_mutex_unlock(&counter_mutex);
        usleep(10000); // Sleep for 10 milliseconds to increase interleaving.
    }
    pthread_exit(NULL);
}

int main() {
    printf("\n");
    const int NUM_THREADS = 5;
    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS];
    int status;

    // Seed the random number generator (not essential for mutex, but good practice).
    srand((unsigned)time(NULL));

    printf("[Main] Creating %d threads, each increments counter 100 times WITH mutex.\n", NUM_THREADS);
    printf("[Main] Compare with lesson-12/04-pthread_join.c (same thing, NO mutex).\n\n");

    for (int i = 0; i < NUM_THREADS; i++) {
        thread_ids[i] = i + 1;
        status = pthread_create(&threads[i], NULL, increment_counter, (void *) &thread_ids[i]);
        if (status != 0) {
            fprintf(stderr, "[Main] ERROR: pthread_create failed for thread %d\n", i + 1);
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    int expected = NUM_THREADS * 100;
    printf("\n══════════════════════════════════════\n");
    printf("  Results (with mutex)\n");
    printf("══════════════════════════════════════\n");
    printf("  Expected: %d (%d threads × 100)\n", expected, NUM_THREADS);
    printf("  Actual:   %d %s\n", counter, counter == expected ? "✓ ALWAYS correct with mutex!" : "← BUG!");

    // Destroy the mutex (good practice when done).
    pthread_mutex_destroy(&counter_mutex);
    return EXIT_SUCCESS;
}
