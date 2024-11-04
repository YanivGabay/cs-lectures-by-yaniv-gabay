#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int read_line(char **line, size_t *len);
int parse_grades_and_name(char *line, int **grades, char *name);
void print_results(const char *name, const int *grades, int numGrades);
void free_resources(int *grades, char *line);




//important details
// if you pass a char * to a function,
// the function will not be able to change the pointer itself
// and will modify a copy of the pointer
// if you want to modify the pointer itself, you need to pass a char **
//same with grades


int main() {
    char *line = NULL;   // Pointer to the buffer allocated by getline
    size_t len = 0;      // Size of the allocated buffer
    int *grades = NULL;  // Array to store grades
    char name[100];      // String to store the name
    int numGrades;

    if (read_line(&line, &len) == -1) {
        printf("Failed to read input\n");
        return 1; // Exit if input reading fails
    }

    numGrades = parse_grades_and_name(line, &grades, name);
    print_results(name, grades, numGrades);
    free_resources(grades, line);

    return 0;
}

int read_line(char **line, size_t *len) {
    printf("Enter grades followed by a name: ");
    size_t read = getline(line, len, stdin);
    return (read != -1) ? 0 : -1;
}

int parse_grades_and_name(char *line, int **grades, char *name) {
    char *token = strtok(line, " \n");
    int numGrades = 0;
    while (token != NULL) {
        int grade;
        if (sscanf(token, "%d", &grade) == 1) {
            int *newGrades = realloc(*grades, (numGrades + 1) * sizeof(int));
            if (newGrades == NULL) {
                printf("Failed to allocate memory for grades\n");
                free(*grades);
                return -1;
            }
            *grades = newGrades;
            (*grades)[numGrades++] = grade;
        } else {
            strcpy(name, token);
            while ((token = strtok(NULL, "\n")) != NULL) {
                strcat(name, " ");
                strcat(name, token);
            }
            break;
        }
        token = strtok(NULL, " \n");
    }
    return numGrades;
}

void print_results(const char *name, const int *grades, int numGrades) {
    printf("Name: %s\n", name);
    printf("Grades: ");
    for (int i = 0; i < numGrades; i++) {
        printf("%d ", grades[i]);
    }
    printf("\n");
}

void free_resources(int *grades, char *line) {
    free(grades);
    free(line);
}
