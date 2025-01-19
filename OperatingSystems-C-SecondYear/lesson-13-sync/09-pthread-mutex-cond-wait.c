// File: producer_consumer_mutex.c
// Compile with: gcc -Wall -pthread producer_consumer_mutex.c -o producer_consumer_mutex

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define BUFFER_SIZE 5
#define NUM_ITEMS 10

// Shared circular buffer and related variables
int buffer[BUFFER_SIZE];
int count = 0;       // Number of items in the buffer
int in_index = 0;    // Next insertion index
int out_index = 0;   // Next removal index

// Mutex and condition variables for synchronization
pthread_mutex_t buffer_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t not_empty = PTHREAD_COND_INITIALIZER;
pthread_cond_t not_full = PTHREAD_COND_INITIALIZER;

// Producer thread function
void* producer(void *arg) {
    for (int i = 1; i <= NUM_ITEMS; i++) {
        pthread_mutex_lock(&buffer_mutex);
        // Wait until the buffer is not full
        while (count == BUFFER_SIZE) {
            pthread_cond_wait(&not_full, &buffer_mutex);
        }
        // Insert item into the buffer
        buffer[in_index] = i;
        in_index = (in_index + 1) % BUFFER_SIZE;
        count++;
        printf("Producer produced: %d, count = %d\n", i, count);
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
            pthread_cond_wait(&not_empty, &buffer_mutex);
        }
        // Remove item from the buffer
        item = buffer[out_index];
        out_index = (out_index + 1) % BUFFER_SIZE;
        count--;
        printf("Consumer consumed: %d, count = %d\n", item, count);
        // Signal that the buffer is not full now
        pthread_cond_signal(&not_full);
        pthread_mutex_unlock(&buffer_mutex);
        sleep(2); // Simulate consumption delay
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t prod_thread, cons_thread;
    int status;

    // Create the producer thread.
    status = pthread_create(&prod_thread, NULL, producer, NULL);
    if (status != 0) {
        fprintf(stderr, "pthread_create for producer failed\n");
        exit(EXIT_FAILURE);
    }

    // Create the consumer thread.
    status = pthread_create(&cons_thread, NULL, consumer, NULL);
    if (status != 0) {
        fprintf(stderr, "pthread_create for consumer failed\n");
        exit(EXIT_FAILURE);
    }

    // Wait for the threads to complete.
    pthread_join(prod_thread, NULL);
    pthread_join(cons_thread, NULL);

    // Clean up resources.
    pthread_mutex_destroy(&buffer_mutex);
    pthread_cond_destroy(&not_empty);
    pthread_cond_destroy(&not_full);
    
    printf("Producer and Consumer finished processing.\n");
    return EXIT_SUCCESS;
}
