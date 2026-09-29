/*
 * 02-pthread_exit.c — pthread_exit with return values
 *
 * Key concepts: pthread_exit, returning data from threads, void* return
 * Compile: gcc -o exit 02-pthread_exit.c -lpthread
 * Run:     ./prog
 */
// File: pthread_ret_val.c
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

// Thread function prototype
void* my_func(void *arg);

int main() {

    printf("\n");
    printf("══════════════════════════════════════\n");
    printf("  pthread_exit with return values\n");
    printf("══════════════════════════════════════\n\n");
    printf("\n");
    pthread_t thread_id;
    int a[5] = {17, 38, 79, 3879, 0}; // Data for the thread
    int i;
    int *ret_val;
    int status;

    srand((unsigned) time(NULL));

    printf("[Main] Input array: ");
    for (i = 0; a[i] != 0; i++) printf("%d ", a[i]);
    printf("\n\n");

    // Create a thread passing array 'a'
    printf("[Main] Creating thread to negate all values...\n");
    status = pthread_create(&thread_id, NULL, my_func, a);
    if (status != 0) {
        fputs("[Main] ERROR: pthread_create failed!\n", stderr);
        exit(EXIT_FAILURE);
    }
    // Wait for the thread to finish and collect its return value
    printf("[Main] Waiting for thread to finish (pthread_join)...\n");
    pthread_join(thread_id, (void **) &ret_val);

    // Print the returned values
    printf("\n[Main] Thread finished! Returned array: ");
    for (i = 0; ret_val[i] != 0; i++) {
        printf("%d ", ret_val[i]);
    }
    printf("\n[Main] The thread allocated memory, returned it via pthread_exit, and main read it via pthread_join.\n");

    free(ret_val);
    return EXIT_SUCCESS;
}

void* my_func(void *arg) {
    int i, argc;
    int *params = (int *) arg;
    int *ret_val;

    printf("[Thread] Received input: ");
    for (i = 0; params[i] != 0; i++)
        printf("%d ", params[i]);
    printf("\n[Thread] Negating each value...\n");

    argc = i + 1;
    ret_val = (int *) malloc(argc * sizeof(int));
    if (ret_val == NULL) {
        perror("cannot allocate memory");
        exit(EXIT_FAILURE);
    }

    // Create an output array by negating input values
    for (i = 0; i < argc; i++)
        ret_val[i] = -params[i];

    // Exit the thread, returning the output array
    pthread_exit(ret_val);
}
