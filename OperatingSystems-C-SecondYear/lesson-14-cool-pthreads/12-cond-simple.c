/*
 * 12-cond-simple.c — Simple correct condition variable usage
 *
 * Key concepts: pthread_cond_wait in while loop, predicate check pattern
 * Compile: gcc -o cond_ok 12-cond-simple.c -lpthread
 * Run:     ./cond_ok
 */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

// Spurious wakeups: https://en.wikipedia.org/wiki/Spurious_wakeup
// This is why we use while() not if() around cond_wait!

#define NUM_THREADS 20

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int condition_met = 0;

void* thread_func(void* arg) {
    long id = (long)arg;

    pthread_mutex_lock(&mutex);
    while (!condition_met) {
        // while-loop protects against spurious wakeups:
        // if a thread wakes without condition_met, it goes back to sleep
        pthread_cond_wait(&cond, &mutex);
    }
    pthread_mutex_unlock(&mutex);

    printf("[Thread %2ld] Condition met — proceeding!\n", id);
    return NULL;
}

int main() {
    printf("\n");
    printf("══════════════════════════════════════\n");
    printf("  Correct Condition Variable Usage\n");
    printf("══════════════════════════════════════\n\n");
    printf("[Main] Creating %d threads. All will wait on cond_wait.\n", NUM_THREADS);
    printf("[Main] After 2 seconds, main sets condition=1 and signals each one.\n\n");

    pthread_t threads[NUM_THREADS];

    for (long i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, thread_func, (void*)i);
    }
    printf("[Main] All %d threads created and waiting.\n", NUM_THREADS);

    sleep(2);

    printf("\n[Main] Setting condition_met = 1\n");
    pthread_mutex_lock(&mutex);
    condition_met = 1;
    pthread_mutex_unlock(&mutex);

    printf("[Main] Sending cond_signal to each thread (one at a time)...\n\n");
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_cond_signal(&cond);
        usleep(100000); // Small delay to observe sequential waking
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);

    printf("\n[Main] All %d threads finished. Compare with 11-5 (the broken version).\n", NUM_THREADS);
    return 0;
}
