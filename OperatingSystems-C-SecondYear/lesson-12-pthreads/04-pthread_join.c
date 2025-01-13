// File: pthread_counter.c
// Compile with: gcc -Wall -pthread pthread_counter.c -o pthread_counter

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

// Global counter (shared among threads)
int counter = 0;

// Thread function prototype
void* increment_counter(void *arg);

int main() {
    const int num_threads = 5;
    pthread_t threads[num_threads];
    int status, i;

    // Seed the random number generator for demonstration purposes.
    srand((unsigned)time(NULL));

    // Create multiple threads
    for (i = 0; i < num_threads; i++) {
        // Create a thread; each thread executes increment_counter.
        int *arg = malloc(sizeof(int));
        *arg = i;  // Pass a unique value for each thread
        status = pthread_create(&threads[i], NULL, increment_counter, (void *)arg);
        if (status != 0) {
            fprintf(stderr, "pthread_create failed: %d\n", status);
            exit(EXIT_FAILURE);
        }
    }

    // Wait for all threads to finish
    for (i = 0; i < num_threads; i++) {
        status = pthread_join(threads[i], NULL);
        if (status != 0) {
            fprintf(stderr, "pthread_join failed: %d\n", status);
            exit(EXIT_FAILURE);
        }
    }

    // Print the resulting counter value
    printf("Final counter value: %d\n", counter);
    return EXIT_SUCCESS;
}

void* increment_counter(void *arg) {
    int id = *((int *) arg);
    // Each thread increments the counter 10 times.
    for (int j = 0; j < 10; j++) {
        // (Warning: This race condition is intentional for demonstration.)
        counter++; 
        printf("Thread %d incremented counter to %d\n", id, counter);
        sleep(rand() % 2);
    }
    // Exit and return (pthread_exit is implicit on function return)
    pthread_exit(NULL);
}
