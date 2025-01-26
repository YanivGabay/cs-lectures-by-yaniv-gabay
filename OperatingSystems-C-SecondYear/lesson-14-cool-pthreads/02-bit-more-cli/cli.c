// File: cli_loader_simulator.c
// Compile with: gcc -Wall -pthread cli_loader_simulator.c -o cli_loader_simulator

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <signal.h>
#include <errno.h>
// Constants
#define MAX_FILES 5
#define PROGRESS_BAR_WIDTH 50

// ====================== Shared Resources ====================== //

// Structure to hold file loading information
typedef struct {
    char filename[100];
    int progress;          // Progress percentage (0-100)
    bool is_loading;       // Indicates if the file is currently loading
    bool is_loaded;        // Indicates if the file has finished loading
} FileLoad;

// Array to hold information about files being loaded
FileLoad files[MAX_FILES];
int num_files = 0;         // Number of files to load

// Mutex to protect access to the files array
pthread_mutex_t files_mutex = PTHREAD_MUTEX_INITIALIZER;

// Flag to indicate program termination
volatile sig_atomic_t terminate_program = 0;

// ====================== Function Prototypes ====================== //

void handle_sigint(int sig);
void display_menu();
void initialize_files();
void *file_loader(void *arg);
void update_display();

// ====================== Signal Handler ====================== //

/**
 * Handles SIGINT (Ctrl+C) to gracefully terminate the program.
 */
void handle_sigint(int sig) {
    terminate_program = 1;
}

// ====================== Helper Functions ====================== //

/**
 * Displays the main menu.
 */
void display_menu() {
    printf("\n========== Multithreaded File Loader ==========\n");
    printf("Commands:\n");
    printf("1. load   - Simulate loading multiple files\n");
    printf("2. status - Display loading progress\n");
    printf("3. exit   - Exit the program\n");
    printf("Enter command number: ");
    fflush(stdout);
}

/**
 * Initializes the files array with dummy filenames.
 */
void initialize_files() {
    pthread_mutex_lock(&files_mutex);
    num_files = MAX_FILES;
    for (int i = 0; i < MAX_FILES; i++) {
        snprintf(files[i].filename, sizeof(files[i].filename), "File_%d.txt", i + 1);
        files[i].progress = 0;
        files[i].is_loading = false;
        files[i].is_loaded = false;
    }
    pthread_mutex_unlock(&files_mutex);
}

/**
 * Generates a progress bar string based on the current progress.
 * @param progress The current progress percentage.
 * @return A string representing the progress bar.
 */
char* get_progress_bar(int progress) {
    static char bar[PROGRESS_BAR_WIDTH + 1];
    int pos = (progress * PROGRESS_BAR_WIDTH) / 100;
    for (int i = 0; i < PROGRESS_BAR_WIDTH; ++i) {
        if (i < pos)
            bar[i] = '#';
        else
            bar[i] = '-';
    }
    bar[PROGRESS_BAR_WIDTH] = '\0';
    return bar;
}

// ====================== Thread Function ====================== //

/**
 * Simulates loading a file by incrementing its progress.
 * @param arg Pointer to the file index in the files array.
 */
void *file_loader(void *arg) {
    int file_index = *((int *)arg);
    free(arg); // Free the allocated memory for argument

    pthread_mutex_lock(&files_mutex);
    files[file_index].is_loading = true;
    pthread_mutex_unlock(&files_mutex);

    while (1) {
        sleep(1); // Simulate time taken to load

        pthread_mutex_lock(&files_mutex);
        if (files[file_index].progress < 100) {
            files[file_index].progress += rand() % 20 + 1; // Increment progress by 1-20%
            if (files[file_index].progress > 100)
                files[file_index].progress = 100;
        }

        if (files[file_index].progress >= 100) {
            files[file_index].is_loading = false;
            files[file_index].is_loaded = true;
            pthread_mutex_unlock(&files_mutex);
            break;
        }
        pthread_mutex_unlock(&files_mutex);
    }

    pthread_exit(NULL);
}

// ====================== Display Function ====================== //

/**
 * Updates the CLI display with current progress of all files.
 */
void update_display() {
    pthread_mutex_lock(&files_mutex);
    printf("\033[H\033[J"); // Clear the screen using ANSI escape codes
    printf("========== File Loading Progress ==========\n");
    for (int i = 0; i < num_files; i++) {
        printf("%s: [%s] %d%%\n", files[i].filename, get_progress_bar(files[i].progress), files[i].progress);
    }
    printf("\nPress Ctrl+C to exit.\n");
    pthread_mutex_unlock(&files_mutex);
}

// ====================== Main Function ====================== //

int main() {
    // Seed random number generator
    srand(time(NULL));

    // Install signal handler for SIGINT
    signal(SIGINT, handle_sigint);

    // Initialize files
    initialize_files();

    // Clear the screen initially
    printf("\033[H\033[J");

    while (!terminate_program) {
        display_menu();

        char input[10];
        if (fgets(input, sizeof(input), stdin) == NULL) {
            if (errno == EINTR && terminate_program) {
                break;
            }
            continue;
        }

        // Remove newline character
        input[strcspn(input, "\n")] = '\0';

        switch (input[0]) {
            case '1':
            case 'l': // 'l' for 'load'
                // Check if files are already being loaded
                {
                    bool already_loading = false;
                    pthread_mutex_lock(&files_mutex);
                    for (int i = 0; i < num_files; i++) {
                        if (files[i].is_loading) {
                            already_loading = true;
                            break;
                        }
                    }
                    pthread_mutex_unlock(&files_mutex);

                    if (already_loading) {
                        printf("Files are already being loaded.\n");
                        break;
                    }

                    // Initialize files
                    initialize_files();

                    printf("Starting to load %d files...\n", num_files);

                    // Create threads to load files
                    for (int i = 0; i < num_files; i++) {
                        pthread_t thread;
                        int *arg = malloc(sizeof(*arg));
                        if (arg == NULL) {
                            fprintf(stderr, "Couldn't allocate memory for thread arg.\n");
                            exit(EXIT_FAILURE);
                        }
                        *arg = i;
                        int status = pthread_create(&thread, NULL, file_loader, arg);
                        if (status != 0) {
                            fprintf(stderr, "Error creating thread for %s\n", files[i].filename);
                            free(arg);
                        } else {
                            pthread_detach(thread); // Detach thread as we don't need to join
                        }
                    }

                    // Continuously update display until all files are loaded
                    while (1) {
                        update_display();

                        // Check if all files are loaded
                        bool all_loaded = true;
                        pthread_mutex_lock(&files_mutex);
                        for (int i = 0; i < num_files; i++) {
                            if (!files[i].is_loaded) {
                                all_loaded = false;
                                break;
                            }
                        }
                        pthread_mutex_unlock(&files_mutex);

                        if (all_loaded || terminate_program) {
                            break;
                        }

                        sleep(1); // Update every second
                    }

                    if (terminate_program) {
                        break;
                    }

                    printf("\nAll files have been loaded successfully!\n");
                }
                break;

            case '2':
            case 's': // 's' for 'status'
                update_display();
                break;

            case '3':
            case 'e': // 'e' for 'exit'
                printf("Exiting program...\n");
                terminate_program = 1;
                break;

            default:
                printf("Unknown command. Please try again.\n");
                break;
        }
    }

    // Cleanup: Ensure all loading threads are terminated
    pthread_mutex_lock(&files_mutex);
    for (int i = 0; i < num_files; i++) {
        // In this simulation, threads will finish naturally.
        // If threads were blocking on conditions, you would signal them here.
    }
    pthread_mutex_unlock(&files_mutex);

    // Destroy the mutex
    pthread_mutex_destroy(&files_mutex);

    printf("Program terminated gracefully.\n");
    return EXIT_SUCCESS;
}
