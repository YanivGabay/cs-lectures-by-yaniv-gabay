// File: pthread_ret_val.c
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

// Thread function prototype
void* my_func(void *arg);

int main() {
    pthread_t thread_id;
    int a[5] = {17, 38, 79, 3879, 0}; // Data for the thread
    int i;
    int *ret_val;
    int status;

    srand((unsigned) time(NULL));

    // Create a thread passing array 'a'
    status = pthread_create(&thread_id, NULL, my_func, a);
    if (status != 0) {
        fputs("pthread_create failed in main\n", stderr);
        exit(EXIT_FAILURE);
    }
    // Wait for the thread to finish and collect its return value
    pthread_join(thread_id, (void **) &ret_val);

    // Print the returned values
    for (i = 0; ret_val[i] != 0; i++) {
        printf(" %d", ret_val[i]);
    }
    putchar('\n');

    free(ret_val);
    return EXIT_SUCCESS;
}

void* my_func(void *arg) {
    int i, argc;
    int *params = (int *) arg;
    int *ret_val;

    // Print the input values
    for (i = 0; params[i] != 0; i++)
        printf("%d ", params[i]);
    putchar('\n');

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
