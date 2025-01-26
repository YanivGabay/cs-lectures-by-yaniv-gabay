// File: multithreaded_drawing_with_display.c
// Compile with: gcc -Wall -pthread -o multithreaded_drawing_with_display multithreaded_drawing_with_display.c

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

// Canvas dimensions
#define CANVAS_WIDTH 20
#define CANVAS_HEIGHT 10

// Number of drawing threads
#define NUM_DRAWING_THREADS 5

// Number of rectangles each thread will draw
#define RECTANGLES_PER_THREAD 10

// Display refresh interval in microseconds (e.g., 500000 for 0.5 seconds)
#define DISPLAY_INTERVAL 500000

// Structure representing the canvas
typedef struct {
    int width;
    int height;
    char **grid;
} canvas_t;

// Shared canvas
canvas_t canvas;

// Mutex to protect the canvas
pthread_mutex_t canvas_mutex;

// Flag to indicate when drawing is complete
int drawing_complete = 0;

// Function to initialize the canvas
void initialize_canvas(canvas_t *c, int width, int height) {
    c->width = width;
    c->height = height;
    c->grid = malloc(height * sizeof(char*));
    if (c->grid == NULL) {
        perror("Failed to allocate memory for canvas rows");
        exit(EXIT_FAILURE);
    }
    for(int i = 0; i < height; i++) {
        c->grid[i] = malloc(width * sizeof(char));
        if (c->grid[i] == NULL) {
            perror("Failed to allocate memory for canvas columns");
            // Free previously allocated rows before exiting
            for(int j = 0; j < i; j++) {
                free(c->grid[j]);
            }
            free(c->grid);
            exit(EXIT_FAILURE);
        }
        for(int j = 0; j < width; j++) {
            c->grid[i][j] = '.'; // Initialize canvas with dots
        }
    }
}

// Function to free the canvas
void free_canvas(canvas_t *c) {
    for(int i = 0; i < c->height; i++) {
        free(c->grid[i]);
    }
    free(c->grid);
}

// Function to print the canvas
void print_canvas(canvas_t *c) {
    // Clear the terminal screen (works on UNIX-like systems)
    printf("\033[H\033[J");
    for(int i = 0; i < c->height; i++) {
        for(int j = 0; j < c->width; j++) {
            printf("%c ", c->grid[i][j]);
        }
        printf("\n");
    }
}

// Function for drawing threads
void* draw_rectangles(void* arg) {
    long thread_id = (long)arg;
    char thread_char = 'A' + thread_id; // Unique character for each thread

    srand(time(NULL) + thread_id); // Seed randomness differently for each thread

    for(int i = 0; i < RECTANGLES_PER_THREAD; i++) {
        // Generate random rectangle dimensions and position
        int rect_width = rand() % 3 + 2;    // Width between 2 and 4
        int rect_height = rand() % 3 + 2;   // Height between 2 and 4
        int pos_x = rand() % (canvas.width - rect_width + 1);
        int pos_y = rand() % (canvas.height - rect_height + 1);

        // Lock the mutex before modifying the canvas
        pthread_mutex_lock(&canvas_mutex);

        // Draw the rectangle on the canvas
        for(int y = pos_y; y < pos_y + rect_height; y++) {
            for(int x = pos_x; x < pos_x + rect_width; x++) {
                canvas.grid[y][x] = thread_char; // Represent each thread with a unique letter
            }
        }
       // printf("Thread %ld drew a rectangle at (%d, %d) of size %dx%d.\n", thread_id, pos_x, pos_y, rect_width, rect_height);

        // Unlock the mutex after modification
        pthread_mutex_unlock(&canvas_mutex);

        // Simulate time taken to draw
        usleep(1000000); // 1000 ms
    }

    pthread_exit(NULL);
}

// Function for the display thread
void* display_canvas(void* arg) {
    while(1) {
        // Lock the mutex before reading the canvas
        pthread_mutex_lock(&canvas_mutex);

        // Print the current state of the canvas
        print_canvas(&canvas);

        // Check if drawing is complete
        if(drawing_complete) {
            pthread_mutex_unlock(&canvas_mutex);
            break;
        }

        // Unlock the mutex after reading
        pthread_mutex_unlock(&canvas_mutex);

        // Sleep for the defined interval before refreshing
        usleep(DISPLAY_INTERVAL);
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t drawing_threads[NUM_DRAWING_THREADS];
    pthread_t display_thread;
    int rc;

    // Initialize the canvas
    initialize_canvas(&canvas, CANVAS_WIDTH, CANVAS_HEIGHT);

    // Initialize the mutex
    if(pthread_mutex_init(&canvas_mutex, NULL) != 0) {
        perror("Mutex initialization failed");
        free_canvas(&canvas);
        exit(EXIT_FAILURE);
    }

    // Create the display thread
    rc = pthread_create(&display_thread, NULL, display_canvas, NULL);
    if(rc) {
        fprintf(stderr, "Error: Unable to create display thread, return code %d\n", rc);
        pthread_mutex_destroy(&canvas_mutex);
        free_canvas(&canvas);
        exit(EXIT_FAILURE);
    }

    // Create drawing threads
    for(long t = 0; t < NUM_DRAWING_THREADS; t++) {
        rc = pthread_create(&drawing_threads[t], NULL, draw_rectangles, (void*)t);
        if(rc) {
            fprintf(stderr, "Error: Unable to create drawing thread %ld, return code %d\n", t, rc);
            // Signal display thread to terminate
            pthread_mutex_lock(&canvas_mutex);
            drawing_complete = 1;
            pthread_mutex_unlock(&canvas_mutex);
            pthread_join(display_thread, NULL);
            pthread_mutex_destroy(&canvas_mutex);
            free_canvas(&canvas);
            exit(EXIT_FAILURE);
        }
    }

    // Wait for all drawing threads to finish
    for(int t = 0; t < NUM_DRAWING_THREADS; t++) {
        pthread_join(drawing_threads[t], NULL);
    }

    // Signal the display thread to terminate
    pthread_mutex_lock(&canvas_mutex);
    drawing_complete = 1;
    pthread_mutex_unlock(&canvas_mutex);

    // Wait for the display thread to finish
    pthread_join(display_thread, NULL);

    // Final display of the canvas
    print_canvas(&canvas);

    // Destroy the mutex
    pthread_mutex_destroy(&canvas_mutex);

    // Free the canvas memory
    free_canvas(&canvas);

    printf("Multithreaded drawing simulation completed.\n");
    return 0;
}
