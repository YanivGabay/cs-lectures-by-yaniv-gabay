/*
 * 06-pthread-once.c — pthread_once — initialization that runs exactly once
 *
 * Key concepts: pthread_once_t, thread-safe one-time init
 * Compile: gcc -o once 06-pthread-once.c -lpthread
 * Run:     ./prog
 */
// File: pthread_once_example.c
// Compile with: gcc -Wall -pthread pthread_once_example.c -o pthread_once_example

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

void* my_func(void *arg);
void init(); // This function will be called only once

int *arr;
int counter = 0;
pthread_once_t once_control = PTHREAD_ONCE_INIT;

int main() {

    printf("\n");
    printf("══════════════════════════════════════\n");
    printf("  pthread_once\n");
    printf("══════════════════════════════════════\n\n");
    printf("\n");
    pthread_t threads[5];
    int i, status;

    printf("[Main] Creating 5 threads. Each calls pthread_once() to ensure init() runs ONCE.\n");
    printf("[Main] Only the FIRST thread to call pthread_once will execute init().\n\n");

    for (i = 0; i < 5; i++) {
        status = pthread_create(&threads[i], NULL, my_func, NULL);
        if (status != 0) {
            fprintf(stderr, "[Main] ERROR: pthread_create failed\n");
            exit(EXIT_FAILURE);
        }
        printf("[Main] Created thread %d\n", i);
    }

    for (i = 0; i < 5; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("\n[Main] All threads finished. Shared array contents: ");
    for (i = 0; i < counter; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n[Main] init() ran exactly once, even though 5 threads called pthread_once.\n");
    free(arr);
    return EXIT_SUCCESS;
}

void* my_func(void *arg) {
    // Ensure that init() is called exactly once.
    pthread_once(&once_control, init);
    // For demonstration, each thread adds a random number to the shared array.
    arr[counter++] = rand() % 10;
    pthread_exit(NULL);
}

void init() {
    printf("  [init()] This runs EXACTLY ONCE — allocating shared array.\n\n");
    srand(time(NULL));
    arr = (int *) malloc(5 * sizeof(int));
    if (!arr) {
        perror("malloc() failed");
        exit(EXIT_FAILURE);
    }
}
