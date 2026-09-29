/*
 * 05-pthread_ping_pong.c — Ping-pong between threads using busy waiting
 *
 * Key concepts: Shared flag, busy waiting (spin loop), thread coordination
 * Compile: gcc -o pingpong 05-pthread_ping_pong.c -lpthread
 * Run:     ./prog
 */
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

    printf("\n");
    printf("══════════════════════════════════════\n");
    printf("  Ping-pong between threads using busy waiting\n");
    printf("══════════════════════════════════════\n\n");
    printf("\n");
    pthread_t threads[2];
    int ids[2] = {0, 1};
    int status;

    // Seed random number generator.
    srand((unsigned) time(NULL));

    printf("[Main] Two threads take turns printing — coordinated via a shared 'turn' flag.\n");
    printf("[Main] Thread 0 = Ping, Thread 1 = Pong. Each waits in a spin loop (busy wait).\n");
    printf("[Main] Note: busy waiting wastes CPU! Better approaches: mutex + cond_wait (lesson 13).\n\n");

    for (int i = 0; i < 2; i++) {
        status = pthread_create(&threads[i], NULL, pingpong, &ids[i]);
        if (status != 0) {
            fprintf(stderr, "[Main] ERROR: pthread_create failed (rc=%d)\n", status);
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < 2; i++) {
        pthread_join(threads[i], NULL);
    }
    printf("\n[Main] Ping-Pong complete — 5 rounds each, perfectly alternating.\n");
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
            printf("[Thread 0] Ping! (round %d)\n", round + 1);
        else
            printf("[Thread 1]   Pong! (round %d)\n", round + 1);

        // Let the other thread proceed.
        turn = 1 - id;
        sleep(1); // Simulate work
    }
    pthread_exit(NULL);
}
