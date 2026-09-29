/*
 * 04-pthread_join.c — Joining threads and shared data races
 *
 * Demonstrates: pthread_join() to wait for threads, shared global variable
 * Key concepts: join blocks until thread finishes, race condition on counter
 * Compile: gcc -Wall -o join 04-pthread_join.c -lpthread
 * Run:     ./join
 *
 * WARNING: The counter increment is NOT thread-safe (no mutex).
 *          Final value should be 50 (5 threads * 10 increments) but might not be!
 *          This is intentional — see lesson 13 for the fix with mutexes.
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>   // for pthread_create(), pthread_join(), pthread_exit()
#include <unistd.h>    // for sleep()
#include <time.h>

// Global counter — shared across ALL threads (this is the race condition)
int counter = 0;

// Thread function prototype
void* increment_counter(void *arg);

int main() {
    printf("\n");
    const int num_threads = 5;
    pthread_t threads[num_threads];
    int status, i;

    // Seed the random number generator for demonstration purposes.
    srand((unsigned)time(NULL));

    printf("[Main] Creating %d threads, each will increment a shared counter 10 times.\n", num_threads);
    printf("[Main] NO mutex is used — watch for the race condition!\n\n");

    // Create multiple threads
    for (i = 0; i < num_threads; i++) {
        int *arg = malloc(sizeof(int));
        *arg = i;
        status = pthread_create(&threads[i], NULL, increment_counter, (void *)arg);
        if (status != 0) {
            fprintf(stderr, "[Main] ERROR: pthread_create failed (rc=%d)\n", status);
            exit(EXIT_FAILURE);
        }
        printf("[Main] Created thread %d\n", i);
    }

    printf("\n[Main] All threads created. Calling pthread_join on each...\n\n");

    // Wait for all threads to finish
    for (i = 0; i < num_threads; i++) {
        status = pthread_join(threads[i], NULL);
        if (status != 0) {
            fprintf(stderr, "[Main] ERROR: pthread_join failed (rc=%d)\n", status);
            exit(EXIT_FAILURE);
        }
    }

    int expected = num_threads * 10;
    printf("\n══════════════════════════════════════\n");
    printf("  Results\n");
    printf("══════════════════════════════════════\n");
    printf("  Expected counter: %d (%d threads × 10 increments)\n", expected, num_threads);
    printf("  Actual counter:   %d %s\n", counter, counter != expected ? "← RACE CONDITION!" : "← correct (got lucky this time)");
    return EXIT_SUCCESS;
}

void* increment_counter(void *arg) {
    int id = *((int *) arg);
    // Each thread increments the counter 10 times.
    for (int j = 0; j < 10; j++) {
        // (Warning: This race condition is intentional for demonstration.)
        counter++; 
        printf("[Thread %d] counter++ → %d (NO mutex — race condition possible!)\n", id, counter);
        sleep(rand() % 2);
    }
    // Exit and return (pthread_exit is implicit on function return)
    pthread_exit(NULL);
}
