/*
 * 03-pthread_cleanup.c — Thread cleanup handlers — pthread_cleanup_push/pop
 *
 * Key concepts: Cleanup handlers run on thread cancellation or exit
 * Compile: gcc -o cleanup 03-pthread_cleanup.c -lpthread
 * Run:     ./cleanup
 */
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

void *thread_func(void *parm);
void cleanup_malloc(void *arg);
void cleanup_msg(void *arg);

int main(int argc, char **argv) {
    pthread_t thread;
    int rc;
    srand(time(NULL));

    printf("\n");
    printf("══════════════════════════════════════\n");
    printf("  pthread_cleanup — Cleanup Handlers\n");
    printf("══════════════════════════════════════\n\n");
    printf("[Main] Cleanup handlers are functions that run automatically\n");
    printf("       when a thread is cancelled or calls pthread_exit.\n\n");

    printf("[Main] Creating secondary thread...\n");
    rc = pthread_create(&thread, NULL, thread_func, NULL);
    if (rc) {
        fprintf(stderr, "[Main] ERROR: pthread_create() failed (rc=%d)\n", rc);
        exit(EXIT_FAILURE);
    }

    sleep(2);
    printf("\n[Main] Cancelling the thread with pthread_cancel()...\n");
    printf("[Main] This should trigger the cleanup handlers.\n");
    rc = pthread_cancel(thread);
    sleep(2);
    printf("\n[Main] Finishing. Note: using return instead of pthread_exit —\n");
    printf("       the secondary thread may not finish its cleanup!\n");
    //HERE there might be race condition
    // if we return EXIT_SUCCESS, the main thread will exit before the secondary thread
    // if we use pthread_exit(NULL), the main thread will wait for the secondary thread to finish
    return EXIT_SUCCESS;
    pthread_exit(NULL);
}

void *thread_func(void *parm) {
    printf("[Thread] Started. Registering cleanup handlers...\n");
    char *p = (char *) malloc(10 * sizeof(char));
    pthread_cleanup_push(cleanup_msg, (void *) "thank you, and come again");
    pthread_cleanup_push(cleanup_malloc, (void *) p);
    if (p == NULL) {
        fprintf(stderr, "[Thread] ERROR: malloc failed!\n");
        exit(EXIT_FAILURE);
    }
    printf("[Thread] Cleanup handlers registered (2 handlers on stack).\n");
    printf("[Thread] Running... (will be cancelled by main after ~2 seconds)\n\n");
    while (1) {
        printf("[Thread] Still running...\n");
        if (rand() % 10 < 1)
        //if (1)
        //we WONT be able to cancel the thread
        //if we return NULL,
        //the cleanup handlers will not run
            return NULL; // Will not execute cleanup; use pthread_exit for cleanup handlers to run.
        sleep(1);
    }
    pthread_cleanup_pop(0);
    pthread_cleanup_pop(0);
    return NULL;
}

void cleanup_msg(void *arg) {
    printf("\n  [Cleanup Handler 1] cleanup_msg called!\n");
    printf("  [Cleanup Handler 1] Message: \"%s\"\n", (char *) arg);
}

void cleanup_malloc(void *arg) {
    printf("  [Cleanup Handler 2] cleanup_malloc called — freeing memory.\n");
    free((char *) arg);
}
