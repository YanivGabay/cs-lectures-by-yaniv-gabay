/*
 * 09-pthread-mutex-cond-wait.c — Producer-Consumer with condition variables
 *
 * Demonstrates: The classic bounded-buffer producer-consumer pattern
 * Key concepts: pthread_cond_wait() (must be in a while loop!), pthread_cond_signal(),
 *               mutex + condition variable coordination, circular buffer
 * Compile: gcc -Wall -o prodcons 09-pthread-mutex-cond-wait.c -lpthread
 * Run:     ./prodcons
 *
 * Pattern:
 *   pthread_mutex_lock(&mutex);
 *   while (condition_not_met)          // MUST be while, not if (spurious wakeups!)
 *       pthread_cond_wait(&cond, &mutex);  // atomically: unlock mutex + sleep + re-lock on wake
 *   // ... critical section ...
 *   pthread_cond_signal(&other_cond);  // wake one waiting thread
 *   pthread_mutex_unlock(&mutex);
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>   // for mutex, cond, thread functions
#include <unistd.h>    // for sleep()

#define BUFFER_SIZE 5   // Max items the buffer can hold
#define NUM_ITEMS 10    // Total items to produce/consume

// Shared circular buffer
int buffer[BUFFER_SIZE];
int count = 0;       // Current number of items in buffer
int in_index = 0;    // Producer inserts at this index
int out_index = 0;   // Consumer removes from this index

// Synchronization primitives
pthread_mutex_t buffer_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t not_empty = PTHREAD_COND_INITIALIZER;  // signaled when buffer has items
pthread_cond_t not_full = PTHREAD_COND_INITIALIZER;   // signaled when buffer has space

// Producer thread function
void* producer(void *arg) {
    for (int i = 1; i <= NUM_ITEMS; i++) {
        pthread_mutex_lock(&buffer_mutex);
        // Wait until the buffer is not full
        while (count == BUFFER_SIZE) {
            printf("[Producer] Buffer FULL (%d/%d) — calling cond_wait...\n", count, BUFFER_SIZE);
            pthread_cond_wait(&not_full, &buffer_mutex);
            printf("[Producer] Woke up! Buffer has space now.\n");
        }
        // Insert item into the buffer
        buffer[in_index] = i;
        in_index = (in_index + 1) % BUFFER_SIZE;
        count++;
        printf("[Producer] Produced item %d → buffer[%d] (buffer: %d/%d)\n", i, (in_index - 1 + BUFFER_SIZE) % BUFFER_SIZE, count, BUFFER_SIZE);
        // Signal that the buffer is not empty now
        pthread_cond_signal(&not_empty);
        pthread_mutex_unlock(&buffer_mutex);
        sleep(1); // Simulate production delay
    }
    pthread_exit(NULL);
}

// Consumer thread function
void* consumer(void *arg) {
    int item;
    for (int i = 0; i < NUM_ITEMS; i++) {
        pthread_mutex_lock(&buffer_mutex);
        // Wait until the buffer is not empty
        while (count == 0) {
            printf("[Consumer] Buffer EMPTY (0/%d) — calling cond_wait...\n", BUFFER_SIZE);
            pthread_cond_wait(&not_empty, &buffer_mutex);
            printf("[Consumer] Woke up! Buffer has items now.\n");
        }
        // Remove item from the buffer
        item = buffer[out_index];
        out_index = (out_index + 1) % BUFFER_SIZE;
        count--;
        printf("[Consumer] Consumed item %d ← buffer[%d] (buffer: %d/%d)\n", item, (out_index - 1 + BUFFER_SIZE) % BUFFER_SIZE, count, BUFFER_SIZE);
        // Signal that the buffer is not full now
        pthread_cond_signal(&not_full);
        pthread_mutex_unlock(&buffer_mutex);
        sleep(2); // Simulate consumption delay
    }
    pthread_exit(NULL);
}

int main() {
    printf("\n");
    pthread_t prod_thread, cons_thread;
    int status;

    printf("[Main] Buffer size: %d, producing %d items total.\n", BUFFER_SIZE, NUM_ITEMS);
    printf("[Main] Producer is faster (1s) than consumer (2s) — watch the buffer fill up!\n\n");

    status = pthread_create(&prod_thread, NULL, producer, NULL);
    if (status != 0) {
        fprintf(stderr, "[Main] ERROR: pthread_create for producer failed\n");
        exit(EXIT_FAILURE);
    }

    status = pthread_create(&cons_thread, NULL, consumer, NULL);
    if (status != 0) {
        fprintf(stderr, "[Main] ERROR: pthread_create for consumer failed\n");
        exit(EXIT_FAILURE);
    }

    pthread_join(prod_thread, NULL);
    pthread_join(cons_thread, NULL);

    pthread_mutex_destroy(&buffer_mutex);
    pthread_cond_destroy(&not_empty);
    pthread_cond_destroy(&not_full);

    printf("\n══════════════════════════════════════\n");
    printf("  All %d items produced and consumed!\n", NUM_ITEMS);
    printf("  No race conditions — mutex + cond_wait kept it safe.\n");
    printf("══════════════════════════════════════\n");
    return EXIT_SUCCESS;
}
