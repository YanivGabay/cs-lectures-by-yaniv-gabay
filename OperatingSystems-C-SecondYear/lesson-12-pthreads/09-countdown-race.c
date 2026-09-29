/*
 * 09-countdown-race.c — Thread race: first to count down to zero wins
 *
 * Demonstrates: Race conditions, cleanup handlers, pthread_exit() from main
 * Key concepts: volatile for shared state, pthread_cleanup_push/pop, main exits early
 * Compile: gcc -Wall -o race 09-countdown-race.c -lpthread
 * Run:     ./race
 *
 * Note: The winner check (if winner == -1) is a race condition itself!
 *       Two threads could both see winner==-1 and both declare victory.
 *       A mutex would fix this — see lesson 13.
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>   // for pthread_create/exit/cleanup_push/pop
#include <unistd.h>    // for sleep()
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
    printf("[Thread %d] Cleanup handler called — releasing resources.\n", id);
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
        printf("[Thread %d] Counting down: %d remaining\n", id, count);

        // If another thread already won, exit immediately.
        if (winner != -1 && winner != id) {
            printf("[Thread %d] Thread %d already won — I'm exiting.\n", id, winner);
            pthread_exit(NULL);
        }
    }

    // If count reached zero and no winner is declared yet, this thread wins.
    if (winner == -1) {
        winner = id;
        printf("[Thread %d] *** I WON THE RACE! *** (race condition: no mutex!)\n", id);
    }

    pthread_cleanup_pop(0);
    pthread_exit(NULL);
}

int main() {
    printf("\n");
    printf("========================================\n");
    printf("  Countdown Race Condition\n");
    printf("========================================\n\n");
    const int num_threads = 3;
    pthread_t threads[num_threads];
    thread_arg_t args[num_threads];
    int i, status;

    srand(time(NULL));  // Seed the random number generator

    printf("[Main] Each thread gets a random countdown (5-10 seconds). First to 0 wins!\n");
    printf("[Main] WARNING: the winner check has NO mutex — a race condition on purpose.\n\n");

    for (i = 0; i < num_threads; i++) {
        args[i].id = i;
        args[i].count = 5 + rand() % 6;
        status = pthread_create(&threads[i], NULL, countdown_race, (void *) &args[i]);
        if (status != 0) {
            fprintf(stderr, "[Main] ERROR: pthread_create failed for thread %d\n", i);
            exit(EXIT_FAILURE);
        }
        printf("[Main] Thread %d starts with countdown = %d\n", i, args[i].count);
    }

    printf("\n[Main] Race started! Main thread calling pthread_exit (threads keep running).\n\n");
    pthread_exit(NULL);  

    // Any code after pthread_exit() will not be executed.
    return EXIT_SUCCESS;
}
