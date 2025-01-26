// File: cli_loader_fetch.c
// Compile with: gcc -Wall -pthread cli_loader_fetch.c -o cli_loader_fetch

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <signal.h>

// Shared flag to indicate when fetching is done
bool fetch_complete = false;

// Mutex to protect access to fetch_complete
pthread_mutex_t flag_mutex = PTHREAD_MUTEX_INITIALIZER;

// Loader thread function: displays a spinning loader
void* loader(void* arg) {
    const char spinner[] = "|/-\\";
    int idx = 0;

    while (1) {
        // Lock mutex to safely read fetch_complete
        pthread_mutex_lock(&flag_mutex);
        bool done = fetch_complete;
        pthread_mutex_unlock(&flag_mutex);

        if (done) {
            break;
        }

        // Display spinner
        printf("\rFetching data... %c", spinner[idx % 4]);
        fflush(stdout);
        idx++;
        usleep(100000); // 100 milliseconds
    }

    // Clear the spinner line after completion
    printf("\rFetching data... Done!      \n");
    pthread_exit(NULL);
}

// Worker thread function: simulates data fetching
void* fetch_data(void* arg) {
    // Simulate data fetching with sleep
    printf("Worker: Starting data fetch...\n");
    sleep(5); // Simulate a 5-second data fetch
    printf("Worker: Data fetch completed.\n");

    // Lock mutex to safely update fetch_complete
    pthread_mutex_lock(&flag_mutex);
    fetch_complete = true;
    pthread_mutex_unlock(&flag_mutex);

    pthread_exit(NULL);
}

int main() {
    pthread_t loader_thread, worker_thread;
    int status;

    // Create the loader thread
    status = pthread_create(&loader_thread, NULL, loader, NULL);
    if (status != 0) {
        fprintf(stderr, "Error: Unable to create loader thread\n");
        exit(EXIT_FAILURE);
    }

    // Create the worker thread
    status = pthread_create(&worker_thread, NULL, fetch_data, NULL);
    if (status != 0) {
        fprintf(stderr, "Error: Unable to create worker thread\n");
        exit(EXIT_FAILURE);
    }

    // Wait for both threads to finish
    pthread_join(worker_thread, NULL);
    pthread_join(loader_thread, NULL);

    // Destroy the mutex
    pthread_mutex_destroy(&flag_mutex);

    printf("Main: All tasks completed successfully.\n");
    return EXIT_SUCCESS;
}
