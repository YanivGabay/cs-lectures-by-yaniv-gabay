/*
 * 01-pthread_create.c — Creating your first thread
 *
 * Demonstrates: pthread_create() and pthread_exit()
 * Key concepts: Thread vs process, pthread_self(), shared address space
 * Compile: gcc -o thread1 01-pthread_create.c -lpthread
 * Run:     ./thread1
 *
 * Note: Both the main thread and the new thread run my_func concurrently.
 *       The interleaved output shows real parallelism.
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>   // for pthread_create(), pthread_exit(), pthread_self()
#include <unistd.h>    // for getpid(), sleep()
#include <time.h>      // for time() (seeding rand)

// Thread function prototype
void* my_func(void *arg);

int main() {
    printf("\n");
    printf("========================================\n");
    printf("  pthread_create — Your First Thread\n");
    printf("========================================\n\n");
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
    printf("[Main] Creating a new thread...\n");
    status = pthread_create(&thread_data, NULL, my_func, (void *) &a);
    if (status != 0) {
        fputs("[Main] ERROR: pthread_create failed!\n", stderr);
        exit(EXIT_FAILURE);
    }
    printf("[Main] Thread created (ID: %lu). Now main also calls my_func.\n", (unsigned long)thread_data);
    printf("[Main] Watch the interleaved output — both threads run concurrently!\n\n");

    // Main thread also runs my_func (simulate concurrency)
    my_func((void *)&a);
    puts("This line is never reached because both threads call pthread_exit");
    return EXIT_SUCCESS;
}

void* my_func(void *arg) {
    int i;
    int *val = (int *) arg;
    // Loop 5 times, printing process and thread info
    for (i = 0; i < 5; i++) {
        printf("[Thread %lu] PID=%d, iteration %d/5 (val=%d)\n",
               (unsigned long) pthread_self(), getpid(), i + 1, *val);
        sleep(rand() % 3); // Sleep for a random time between 0-2 seconds
    }
    // Exit the thread
    pthread_exit(NULL);
}
