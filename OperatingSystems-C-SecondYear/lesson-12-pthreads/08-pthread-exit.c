// File: main_exits_but_threads_continue.c
// Compile with: gcc -Wall -pthread main_exits_but_threads_continue.c -o main_exits_but_threads_continue

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

// Thread function: prints its ID and iteration count.
void* thread_func(void *arg) {
    int id = *((int *) arg);
    for (int i = 0; i < 10; i++) {
        printf("Thread %d: iteration %d\n", id, i);
        sleep(1);  // Simulate work by sleeping for 1 second.
    }
    printf("Thread %d: finished\n", id);
    pthread_exit(NULL);
}

int main() {
    pthread_t threads[3];
    int thread_ids[3] = {1, 2, 3};
    
    // Create three threads.
    for (int i = 0; i < 3; i++) {
        if (pthread_create(&threads[i], NULL, thread_func, (void *) &thread_ids[i]) != 0) {
            perror("pthread_create failed");
            exit(EXIT_FAILURE);
        }
    }
    
    // Main thread prints a message and then exits.
    printf("Main thread: finishing early (calling pthread_exit). Threads will continue running.\n");
    
    // Instead of exiting normally (which would end the process and kill all threads),
    // the main thread calls pthread_exit. This causes the main thread to finish its work
    // while keeping the process alive until all non-detached threads complete.
    pthread_exit(NULL);
    
    // Note: Any code after pthread_exit() will not be executed.
    return EXIT_SUCCESS;
}
