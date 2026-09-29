/*
 * 08-pthread-exit.c — Main thread exits early, worker threads continue
 *
 * Key concepts: pthread_exit from main, process stays alive for threads
 * Compile: gcc -o early_exit 08-pthread-exit.c -lpthread
 * Run:     ./prog
 */
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
        printf("[Thread %d] Iteration %d/10 — still running after main exited!\n", id, i + 1);
        sleep(1);
    }
    printf("[Thread %d] Finished all 10 iterations.\n", id);
    pthread_exit(NULL);
}

int main() {

    printf("\n");
    printf("══════════════════════════════════════\n");
    printf("  Main thread exits early, worker threads continue\n");
    printf("══════════════════════════════════════\n\n");
    printf("\n");
    pthread_t threads[3];
    int thread_ids[3] = {1, 2, 3};
    
    printf("[Main] Key concept: if main calls return/exit, ALL threads die.\n");
    printf("[Main] But if main calls pthread_exit, the process stays alive for worker threads!\n\n");

    for (int i = 0; i < 3; i++) {
        if (pthread_create(&threads[i], NULL, thread_func, (void *) &thread_ids[i]) != 0) {
            perror("[Main] ERROR: pthread_create failed");
            exit(EXIT_FAILURE);
        }
        printf("[Main] Created thread %d\n", thread_ids[i]);
    }

    printf("\n[Main] Calling pthread_exit() NOW — main thread stops, but threads keep running!\n");
    printf("[Main] Watch: the thread output continues even though main is gone.\n\n");
    
    // Instead of exiting normally (which would end the process and kill all threads),
    // the main thread calls pthread_exit. This causes the main thread to finish its work
    // while keeping the process alive until all non-detached threads complete.
    pthread_exit(NULL);
    
    // Note: Any code after pthread_exit() will not be executed.
    return EXIT_SUCCESS;
}
