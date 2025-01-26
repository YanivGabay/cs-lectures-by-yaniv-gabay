// File: thread_safe_logging.c
// Compile with: gcc -Wall -pthread -o thread_safe_logging thread_safe_logging.c

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_THREADS 5
#define LOG_ENTRIES_PER_THREAD 1000
#define LOG_FILE "thread_log.txt"

// Mutex to protect file writing
pthread_mutex_t file_mutex;

void* write_log(void* arg) {
    long thread_id = (long)arg;
    FILE *fp;

    for (int i = 0; i < LOG_ENTRIES_PER_THREAD; i++) {
        // Lock the mutex before writing to the file
        pthread_mutex_lock(&file_mutex);
        
        // Critical section: write to the log file
        fp = fopen(LOG_FILE, "a");
        if (fp == NULL) {
            perror("fopen failed");
            pthread_mutex_unlock(&file_mutex);
            pthread_exit(NULL);
        }

        fprintf(fp, "Thread %ld: Log entry %d\n", thread_id, i + 1);
        fclose(fp);
        
        // Unlock the mutex after writing
        pthread_mutex_unlock(&file_mutex);
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NUM_THREADS];
    int rc;

    // Initialize the mutex
    if (pthread_mutex_init(&file_mutex, NULL) != 0) {
        perror("Mutex initialization failed");
        exit(EXIT_FAILURE);
    }

    // Create or truncate the log file
    FILE *fp = fopen(LOG_FILE, "w");
    if (fp == NULL) {
        perror("Initial fopen failed");
        pthread_mutex_destroy(&file_mutex);
        exit(EXIT_FAILURE);
    }
    fclose(fp);

    // Create threads
    for (long t = 0; t < NUM_THREADS; t++) {
        rc = pthread_create(&threads[t], NULL, write_log, (void*)t);
        if (rc) {
            fprintf(stderr, "Error: Unable to create thread %ld, return code %d\n", t, rc);
            pthread_mutex_destroy(&file_mutex);
            exit(EXIT_FAILURE);
        }
    }

    // Wait for all threads to finish
    for (int t = 0; t < NUM_THREADS; t++) {
        pthread_join(threads[t], NULL);
    }

    // Destroy the mutex
    pthread_mutex_destroy(&file_mutex);

    printf("All threads have finished logging.\n");
    return 0;
}
