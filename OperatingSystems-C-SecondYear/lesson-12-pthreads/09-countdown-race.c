// File: countdown_race.c
// Compile with: gcc -Wall -pthread countdown_race.c -o countdown_race

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

// Structure to pass to each thread containing its ID and starting count.
typedef struct {
    int id;
    int count;
} thread_arg_t;

// Global variable to indicate the winner (-1 if none yet).
volatile int winner = -1;

// Cleanup handler: Prints a cleanup message if the thread is canceled.
void cleanup(void *arg) {
    int id = *((int *) arg);
    printf("Thread %d: cleaning up resources...\n", id);
}

// Thread function: each thread counts down and declares victory when it reaches zero.
void* countdown_race(void *arg) {
    thread_arg_t *data = (thread_arg_t *) arg;
    int id = data->id;
    int count = data->count;

    // Register a cleanup handler (for demonstration purposes)
    pthread_cleanup_push(cleanup, (void *) &data->id);

    while (count > 0) {
        sleep(1);  // Simulate work by waiting 1 second
        count--;
        printf("Thread %d: count = %d\n", id, count);

        // If another thread already won, exit immediately.
        if (winner != -1 && winner != id) {
            printf("Thread %d: Exiting, since thread %d has already won.\n", id, winner);
            pthread_exit(NULL);
        }
    }

    // If count reached zero and no winner is declared yet, this thread wins.
    if (winner == -1) {
        winner = id;
        printf("Thread %d: I won the race!\n", id);
    }

    pthread_cleanup_pop(0);
    pthread_exit(NULL);
}

int main() {
    const int num_threads = 3;
    pthread_t threads[num_threads];
    thread_arg_t args[num_threads];
    int i, status;

    srand(time(NULL));  // Seed the random number generator

    // Create threads with a random countdown value between 5 and 10.
    for (i = 0; i < num_threads; i++) {
        args[i].id = i;
        args[i].count = 5 + rand() % 6;  // Random value in [5,10]
        status = pthread_create(&threads[i], NULL, countdown_race, (void *) &args[i]);
        if (status != 0) {
            fprintf(stderr, "pthread_create failed for thread %d\n", i);
            exit(EXIT_FAILURE);
        }
    }

    // Main thread finishes early by calling pthread_exit.
    // This allows the process to remain alive until all worker threads complete.
    printf("Main thread: finishing early while threads continue running.\n");
    pthread_exit(NULL);  

    // Any code after pthread_exit() will not be executed.
    return EXIT_SUCCESS;
}
