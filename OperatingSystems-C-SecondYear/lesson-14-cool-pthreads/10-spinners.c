// File: multithreaded_spinner_animation.c
// Compile with: gcc -Wall -pthread -o multithreaded_spinner_animation multithreaded_spinner_animation.c

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>

// Define the number of spinner threads
#define NUM_SPINNERS 5

// Define spinner characters for animation
const char spinner_chars[] = {'|', '/', '-', '\\'};
const int NUM_SPINNER_CHARS = sizeof(spinner_chars) / sizeof(spinner_chars[0]);

// Structure to pass spinner information to threads
typedef struct {
    int spinner_id;
    int line_number;
} spinner_info_t;

// Mutex to protect terminal writes
pthread_mutex_t terminal_mutex;

// Flag to signal spinner threads to stop
volatile int stop_flag = 0;

// Function to hide the cursor
void hide_cursor() {
    printf("\033[?25l");
}

// Function to show the cursor
void show_cursor() {
    printf("\033[?25h");
}

// Function to clear the screen
void clear_screen() {
    printf("\033[2J");
}

// Function to move the cursor to a specific position
void move_cursor(int line, int column) {
    printf("\033[%d;%dH", line, column);
}

// Spinner thread function
void* spinner_thread(void* arg) {
    spinner_info_t* info = (spinner_info_t*)arg;
    int spinner_id = info->spinner_id;
    int line = info->line_number;
    int spinner_index = 0;

    while(!stop_flag) {
        // Lock the mutex before writing to the terminal
        pthread_mutex_lock(&terminal_mutex);

        // Move cursor to the designated line and column
        move_cursor(line, 1);

        // Print the spinner character
        printf("Spinner %d: %c", spinner_id, spinner_chars[spinner_index]);

        // Flush the output to ensure real-time display
        fflush(stdout);

        // Unlock the mutex after writing
        pthread_mutex_unlock(&terminal_mutex);

        // Update spinner index for next character
        spinner_index = (spinner_index + 1) % NUM_SPINNER_CHARS;

        // Sleep for a short duration to control spinner speed
        usleep(100000); // 100 ms
    }

    // Clear the spinner line upon termination
    pthread_mutex_lock(&terminal_mutex);
    move_cursor(line, 1);
    printf("Spinner %d:   ", spinner_id);
    fflush(stdout);
    pthread_mutex_unlock(&terminal_mutex);

    pthread_exit(NULL);
}

// Controller thread function to stop the animation after a duration
void* controller_thread(void* arg) {
    int duration = *((int*)arg);
    sleep(duration);
    stop_flag = 1;
    pthread_exit(NULL);
}

int main() {
    pthread_t spinners[NUM_SPINNERS];
    pthread_t controller;
    spinner_info_t spinner_infos[NUM_SPINNERS];
    int rc;
    int animation_duration = 5; // Duration in seconds

    // Initialize the mutex
    if(pthread_mutex_init(&terminal_mutex, NULL) != 0) {
        perror("Mutex initialization failed");
        exit(EXIT_FAILURE);
    }

    // Clear the screen and hide the cursor
    clear_screen();
    hide_cursor();

    // Initialize spinner threads
    for(long i = 0; i < NUM_SPINNERS; i++) {
        spinner_infos[i].spinner_id = i + 1;
        spinner_infos[i].line_number = i + 1; // Assign each spinner to its own line
        rc = pthread_create(&spinners[i], NULL, spinner_thread, (void*)&spinner_infos[i]);
        if(rc) {
            fprintf(stderr, "Error: Unable to create spinner thread %ld, return code %d\n", i, rc);
            stop_flag = 1;
            // Join already created threads before exiting
            for(int j = 0; j < i; j++) {
                pthread_join(spinners[j], NULL);
            }
            pthread_mutex_destroy(&terminal_mutex);
            show_cursor();
            exit(EXIT_FAILURE);
        }
    }

    // Create the controller thread
    rc = pthread_create(&controller, NULL, controller_thread, (void*)&animation_duration);
    if(rc) {
        fprintf(stderr, "Error: Unable to create controller thread, return code %d\n", rc);
        stop_flag = 1;
        // Join spinner threads before exiting
        for(int i = 0; i < NUM_SPINNERS; i++) {
            pthread_join(spinners[i], NULL);
        }
        pthread_mutex_destroy(&terminal_mutex);
        show_cursor();
        exit(EXIT_FAILURE);
    }

    // Wait for the controller thread to finish
    pthread_join(controller, NULL);

    // Wait for all spinner threads to finish
    for(int i = 0; i < NUM_SPINNERS; i++) {
        pthread_join(spinners[i], NULL);
    }

    // Show the cursor again before exiting
    show_cursor();

    // Move the cursor below the spinners
    move_cursor(NUM_SPINNERS + 2, 1);
    printf("Animation complete. Exiting...\n");

    // Destroy the mutex
    pthread_mutex_destroy(&terminal_mutex);

    return 0;
}
