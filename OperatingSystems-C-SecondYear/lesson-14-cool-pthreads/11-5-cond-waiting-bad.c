/*
 * 11-5-cond-waiting-bad.c — Condition variable pitfalls — what can go wrong
 *
 * Key concepts: Missed signals, spurious wakeups, why while-loop is needed
 * Compile: gcc -o cond_bad 11-5-cond-waiting-bad.c -lpthread
 * Run:     ./cond_bad
 */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_THREADS 2

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int condition_met = 0;

void* thread_func(void* arg) {
    long id = (long)arg;
    printf("[Thread %ld] Starting — will wait for condition...\n", id);
    pthread_mutex_lock(&mutex);
    while (!condition_met) {
        printf("[Thread %ld] Calling cond_wait (blocking)...\n", id);
        pthread_cond_wait(&cond, &mutex);
        printf("[Thread %ld] Woke up! Checking condition...\n", id);
    }
    pthread_mutex_unlock(&mutex);

    printf("[Thread %ld] Condition met — proceeding.\n", id);
    return NULL;
}

int main() {
    printf("\n");
    printf("══════════════════════════════════════\n");
    printf("  Condition Variable Pitfalls\n");
    printf("══════════════════════════════════════\n\n");
    printf("[Main] BUG DEMO: signals sent BEFORE threads are created!\n");
    printf("[Main] The signals are lost — threads will block forever.\n\n");

    pthread_t threads[NUM_THREADS];

    // BUG: Sending signals before any thread is waiting
    printf("[Main] Sending cond_signal #1... (nobody is listening!)\n");
    pthread_cond_signal(&cond);
    printf("[Main] Sending cond_signal #2... (still nobody listening!)\n");
    pthread_cond_signal(&cond);
    printf("[Main] Both signals are LOST — they don't queue up.\n\n");

    // Create threads after signals — they'll never wake up
    for (long i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, thread_func, (void*)i);
    }

    printf("[Main] Threads created. They're waiting for signals that already fired.\n");
    printf("[Main] This program will hang forever! (Ctrl+C to exit)\n");
    printf("[Main] Fix: set condition_met=1 before signaling, or signal after threads start.\n");

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);

    return 0;
}
