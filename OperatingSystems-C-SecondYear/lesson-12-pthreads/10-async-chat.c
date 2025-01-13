// File: chat_simulator.c
// Compile with: gcc -Wall -pthread chat_simulator.c -o chat_simulator

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#define NUM_PARTICIPANTS 3
#define NUM_MESSAGES 5
#define LOG_SIZE 50

// Global shared log array (for messages)
char log_buffer[LOG_SIZE];
// Global variable indicating next log position
volatile int log_index = 0;

// Thread function: Each "chat participant" posts messages asynchronously.
void* chat_participant(void *arg) {
    int id = *((int *) arg);
    char message[50];

    for (int i = 0; i < NUM_MESSAGES; i++) {
        // Simulate message creation delay.
        sleep(1 + rand() % 2);
        
        // Create a simple message.
        snprintf(message, sizeof(message), "P%d msg %d", id, i);
        
        // Busy waiting: Wait until there is space in the log.
        while (log_index >= LOG_SIZE)
            ; // In a proper system, you would use condition variables
        
        // Post the message in the global log (no mutex used)
        int pos = log_index;
        log_buffer[pos] = '\0'; // Clear previous (not strictly necessary)
        strncpy(log_buffer, message, sizeof(log_buffer)-1);
        log_index++;
        printf("Participant %d posted: %s\n", id, message);
    }
    
    pthread_exit(NULL);
}

int main() {
    pthread_t participants[NUM_PARTICIPANTS];
    int ids[NUM_PARTICIPANTS];
    int status, i;

    srand(time(NULL));

    // Create chat participant threads.
    for (i = 0; i < NUM_PARTICIPANTS; i++) {
        ids[i] = i;
        status = pthread_create(&participants[i], NULL, chat_participant, (void *)&ids[i]);
        if (status != 0) {
            fprintf(stderr, "pthread_create failed for participant %d\n", i);
            exit(EXIT_FAILURE);
        }
    }
    
    // Main thread finishes quickly.
    printf("Main thread: Exiting immediately, chat participants continue.\n");
    pthread_exit(NULL);
    
    return EXIT_SUCCESS;
}
