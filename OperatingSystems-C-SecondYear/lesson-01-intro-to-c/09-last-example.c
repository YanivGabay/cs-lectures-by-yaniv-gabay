
#include <stdio.h>
#include <stdlib.h>

void initializeData(int **data, int rows, int cols);

int main() {
    
    int rows = 3, cols = 4;
    int **data = malloc(rows * sizeof(int*));
    initializeData(data, rows, cols);  // Initialize the 2D array

    // Print the data
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%d ", data[i][j]);
        }
        printf("\n");
    }

    // Free the allocated memory
    for (int i = 0; i < rows; i++) {
        free(data[i]);
    }
    free(data);

    return 0;
}


void initializeData(int **data, int rows, int cols) {
    // Allocate memory for the array of row pointers
    
    if (data == NULL) {
        perror("Failed to allocate memory for row pointers");
        exit(EXIT_FAILURE);
    }

    // Allocate memory for each row and initialize values
    for (int i = 0; i < rows; i++) {
        (data)[i] = malloc(cols * sizeof(int));
        if ((data)[i] == NULL) {
            perror("Failed to allocate memory for rows");
            exit(EXIT_FAILURE);
        }
        for (int j = 0; j < cols; j++) {
            (data)[i][j] = i * cols + j;  // Example data
        }
    }
}
