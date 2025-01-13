// File: pthread_pingpong.c
// Compile with: gcc -Wall -pthread pthread_pingpong.c -o pthread_pingpong

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

// Global variable to indicate whose turn it is (0 or 1)
volatile int turn = 0;

// Thread function prototype
void* pingpong(void *arg);

int main() {
    pthread_t threads[2];
    int ids[2] = {0, 1};
    int status;

    // Seed random number generator.
    srand((unsigned) time(NULL));

    // Create two threads with IDs 0 and 1.
    for (int i = 0; i < 2; i++) {
        status = pthread_create(&threads[i], NULL, pingpong, &ids[i]);
        if (status != 0) {
            fprintf(stderr, "pthread_create failed: %d\n", status);
            exit(EXIT_FAILURE);
        }
    }

    // Wait for both threads to finish (they run a fixed number of rounds)
    for (int i = 0; i < 2; i++) {
        pthread_join(threads[i], NULL);
    }
    printf("Ping-Pong finished.\n");
    return EXIT_SUCCESS;
}

void* pingpong(void *arg) {
    int id = *((int *) arg);
    // Each thread will print its message 5 times.
    for (int round = 0; round < 5; round++) {
        // Busy wait until it is this thread's turn.
        while (turn != id) {
            ; // Do nothing
        }

        // Print message: "Ping" or "Pong"
        if (id == 0)
            printf("Ping\n");
        else
            printf("Pong\n");

        // Let the other thread proceed.
        turn = 1 - id;
        sleep(1); // Simulate work
    }
    pthread_exit(NULL);
}
