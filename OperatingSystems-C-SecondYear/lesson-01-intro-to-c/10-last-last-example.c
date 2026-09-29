/*
 * 10-last-last-example.c — 2D dynamic allocation in C (malloc/free)
 *
 * Key concepts: malloc, free, 2D arrays, pointer-to-pointer
 * Compile: gcc -o prog 10-last-last-example.c
 * Run:     ./prog
 */

#include <stdio.h>
#include <stdlib.h>

void initializeData(int ***data, int rows, int cols);

int main() {

    printf("\n");
    printf("══════════════════════════════════════\n");
    printf("  2D dynamic allocation in C (malloc/free)\n");
    printf("══════════════════════════════════════\n\n");
    
    printf("\n");
    printf("  receives int*** so it can allocate\n");
    printf("  the array itself (modify caller's pointer).\n\n");

    int rows = 3, cols = 4;
    int ** data;
    printf("  Calling initializeData(&data, %d, %d)...\n\n", rows, cols);
    initializeData(&data, rows, cols);

    printf("  Values (data[i][j] = i * %d + j):\n\n", cols);
    printf("       ");
    for (int j = 0; j < cols; j++) printf("col%-2d ", j);
    printf("\n       ");
    for (int j = 0; j < cols; j++) printf("----- ");
    printf("\n");
    for (int i = 0; i < rows; i++) {
        printf("  row%d |", i);
        for (int j = 0; j < cols; j++) {
            printf(" %3d  ", data[i][j]);
        }
        printf("\n");
    }
    printf("\n========================================\n");

    // Free the allocated memory
    for (int i = 0; i < rows; i++) {
        free(data[i]);
    }
    free(data);

    return 0;
}


void initializeData(int ***data, int rows, int cols) {
    // Allocate memory for the array of row pointers
    
    *data = malloc(rows*sizeof(int*));

    if (data == NULL) {
        perror("Failed to allocate memory for row pointers");
        exit(EXIT_FAILURE);
    }

    // Allocate memory for each row and initialize values
    for (int i = 0; i < rows; i++) {
        (*data)[i] = malloc(cols * sizeof(int));
        if ((*data)[i] == NULL) {
            perror("Failed to allocate memory for rows");
            exit(EXIT_FAILURE);
        }
        for (int j = 0; j < cols; j++) {
            (*data)[i][j] = i * cols + j;  // Example data
        }
    }
}
