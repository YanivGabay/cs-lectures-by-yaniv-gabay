#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int main() {
    char *line = NULL;   // Pointer to the buffer allocated by getline
    size_t len = 0;      // Size of the allocated buffer
    size_t read;        // Number of characters read by getline
    int *grades = NULL;  // Array to store grades
    int numGrades = 0;   // Number of grades read
    char name[100];      // String to store the name

    printf("Enter grades followed by a name: ");
    read = getline(&line, &len, stdin); // Read the line
    if (read == -1) {
        printf("Failed to read input\n");
        return 1; // Exit if input reading fails
    }

    // Tokenize the input to separate grades from the name
    char *token = strtok(line, " \n");
    while (token != NULL) {
        int grade;
        if (sscanf(token, "%d", &grade) == 1) {
            // Reallocate grades array to hold new grade
            int *newGrades = realloc(grades, (numGrades + 1) * sizeof(int));
            if (newGrades == NULL) {
                printf("Failed to allocate memory for grades\n");
                free(grades);
                free(line);
                return 1;
            }
            grades = newGrades;
            grades[numGrades++] = grade;
        } else {
            // Assume the token is part of the name
            strcpy(name, token);
            // Collect the remaining part of the name
            while ((token = strtok(NULL, "\n")) != NULL) {
                strcat(name, " ");
                strcat(name, token);
            }
            break;
        }
        token = strtok(NULL, " \n");
    }

    // Output the results
    printf("Name: %s\n", name);
    printf("Grades: ");
    for (int i = 0; i < numGrades; i++) {
        printf("%d ", grades[i]);
    }
    printf("\n");

    // Clean up
    free(grades);
    free(line);

    return 0;

}
