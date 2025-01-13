// File: pthread1.c
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

// Thread function prototype
void* my_func(void *arg);

int main() {
    pthread_t thread_data;
    int a = 1;  // Value passed to thread
    int status;

    // Seed random number generator
    srand((unsigned) time(NULL));

    // Create a new thread running my_func, passing a pointer to 'a'
   /*
   pthread_create(&thread_data, NULL, my_func, (void *)&a)
&thread_data: The thread ID is stored here.
NULL: Using default thread attributes.
my_func: Function executed by the thread.
(void *)&a: The argument passed to the thread (a pointer to a).*/
    status = pthread_create(&thread_data, NULL, my_func, (void *) &a);
    if (status != 0) {
        fputs("pthread_create failed in main\n", stderr);
        exit(EXIT_FAILURE);
    }

    // Main thread also runs my_func with a different parameter (simulate concurrency)
    my_func((void *)&a);
    puts("This line is never reached because both threads call pthread_exit");
    return EXIT_SUCCESS;
}

void* my_func(void *arg) {
    int i;
    int *val = (int *) arg;
    // Loop 5 times, printing process and thread info
    for (i = 0; i < 5; i++) {
        printf("Process = %d, Thread = %d, pthread_self = %u, i = %d\n",
               getpid(), *val, (unsigned int) pthread_self(), i);
        sleep(rand() % 3); // Sleep for a random time between 0-2 seconds
    }
    // Exit the thread
    pthread_exit(NULL);
}
