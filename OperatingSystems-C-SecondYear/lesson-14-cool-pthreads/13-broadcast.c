/*
 * 13-broadcast.c — Broadcast vs Signal — pthread_cond_broadcast
 *
 * Key concepts: pthread_cond_signal (wake one) vs pthread_cond_broadcast (wake all)
 * Compile: gcc -o broadcast 13-broadcast.c -lpthread
 * Run:     ./broadcast
 */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_THREADS 20

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int condition_met = 0;

void* thread_func(void* arg) {
    long id = (long)arg;

    pthread_mutex_lock(&mutex);
    while (!condition_met) {
        pthread_cond_wait(&cond, &mutex);
        // When broadcast fires, ALL threads wake up simultaneously.
        // But only one can hold the mutex at a time — they take turns.
        printf("[Thread %2ld] Woke up! Re-checking condition (mutex acquired)...\n", id);
    }
    pthread_mutex_unlock(&mutex);

    printf("[Thread %2ld] Proceeding — condition is true.\n", id);
    return NULL;
}

int main() {
    printf("\n");
    printf("══════════════════════════════════════\n");
    printf("  Broadcast vs Signal\n");
    printf("══════════════════════════════════════\n\n");
    printf("[Main] cond_signal:    wakes ONE waiting thread\n");
    printf("[Main] cond_broadcast: wakes ALL waiting threads\n\n");
    printf("[Main] Creating %d threads, all will call cond_wait...\n", NUM_THREADS);

    pthread_t threads[NUM_THREADS];

    for (long i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, thread_func, (void*)i);
    }

    sleep(2);

    printf("\n[Main] All threads are waiting. Calling pthread_cond_broadcast!\n");
    printf("[Main] All %d threads will wake up at once (but mutex serializes them).\n\n", NUM_THREADS);

    pthread_mutex_lock(&mutex);
    condition_met = 1;
    pthread_cond_broadcast(&cond);
    pthread_mutex_unlock(&mutex);

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);

    printf("\n[Main] All %d threads finished.\n", NUM_THREADS);
    printf("[Main] Compare with 12-cond-simple.c where we used signal (one at a time).\n");
    return 0;
}
