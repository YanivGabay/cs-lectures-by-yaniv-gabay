// File: pthread_cleanup2.c
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // for sleep()

void *thread_func(void *parm);
void cleanup_malloc(void *arg);
void cleanup_msg(void *arg);

int main(int argc, char **argv) {
    pthread_t thread;
    int rc;
    srand(time(NULL));
    
    puts("creating the thread");
    rc = pthread_create(&thread, NULL, thread_func, NULL);
    if (rc) {
        fputs("pthread_create() failed\n", stderr);
        exit(EXIT_FAILURE);
    }
    
    sleep(2);
    puts("cancelling the thread");
    rc = pthread_cancel(thread);
    sleep(2);
    puts("main finishing");
    //HERE there might be race condition
    // if we return EXIT_SUCCESS, the main thread will exit before the secondary thread
    // if we use pthread_exit(NULL), the main thread will wait for the secondary thread to finish
    return EXIT_SUCCESS;
    pthread_exit(NULL);
}

void *thread_func(void *parm) {
    puts("in secondary thread");
    char *p = (char *) malloc(10 * sizeof(char));
    pthread_cleanup_push(cleanup_msg, (void *) "thank you, and come again");
    pthread_cleanup_push(cleanup_malloc, (void *) p);
    if (p == NULL) {
        puts("malloc failed");
        exit(EXIT_FAILURE);
    }
    while (1) {
        puts("secondary thread run");
        if (rand() % 10 < 1)
            return NULL; // Will not execute cleanup; use pthread_exit for cleanup handlers to run.
        sleep(1);
    }
    pthread_cleanup_pop(0);
    pthread_cleanup_pop(0);
    return NULL;
}

void cleanup_msg(void *arg) {
    puts("In the cleanup_msg");
    puts((char *) arg);
}

void cleanup_malloc(void *arg) {
    puts("In the cleanup_malloc");
    free((char *) arg);
}
