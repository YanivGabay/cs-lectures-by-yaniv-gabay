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
    pthread_t threads[5];
    int i, status;

    // Create 5 threads, all of which must share initialization.
    for (i = 0; i < 5; i++) {
        status = pthread_create(&threads[i], NULL, my_func, NULL);
        if (status != 0) {
            fputs("pthread_create failed in main\n", stderr);
            exit(EXIT_FAILURE);
        }
    }

    for (i = 0; i < 5; i++) {
        pthread_join(threads[i], NULL);
    }
    
    // Print the shared array values.
    for (i = 0; i < counter; i++) {
        printf("%d ", arr[i]);
    }
    printf("\nDone\n");
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
    puts("In init()");
    srand(time(NULL));  // Initialize random number generator
    arr = (int *) malloc(5 * sizeof(int));
    if (!arr) {
        perror("malloc() failed");
        exit(EXIT_FAILURE);
    }
}
