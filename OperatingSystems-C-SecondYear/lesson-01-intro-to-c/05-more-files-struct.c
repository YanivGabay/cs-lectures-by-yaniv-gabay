/*
 * 05-more-files-struct.c — Reading/writing structs to files
 *
 * Key concepts: structs, fwrite, fread, file I/O with structured data
 * Compile: gcc -o prog 05-more-files-struct.c
 * Run:     ./prog
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


const int EMPLOYEE_COUNT = 3;
const int MAX_NAME_LENGTH = 50;


int main() {

    printf("\n");
    printf("══════════════════════════════════════\n");
    printf("  Reading/writing structs to files\n");
    printf("══════════════════════════════════════\n\n");
    //### Althourgh this doesnt compile on my window gcc 
    // but it does compile on the HAC linux gcc environment
    struct Employee {
    int id;
    char name[MAX_NAME_LENGTH];
    float salary;
    };
    //	int array[EMPLOYEE_COUNT];
    // this does compile:
   // int array[EMPLOYEE_COUNT];
   //ALTHOURGH ON MOST GCC ON WINDOWS IT WONT COMPILE

    // Creating an array of employees
   // struct Employee employees[EMPLOYEE_COUNT] = {
  //      {1, "Alice", 55000.00},
   //     {2, "Bob", 60000.00},
   //     {3, "Charlie", 50000.00}
  //  };
  // we cant use const with object variables so we do dynamic allocation:
    struct Employee *employees = (struct Employee *)malloc(EMPLOYEE_COUNT * sizeof(struct Employee));
    if (employees == NULL) {
        perror("Unable to allocate memory");
        return 1;
    }
    // now put the values in the array:
    employees[0].id = 1;
    strcpy(employees[0].name, "Alice");
    employees[0].salary = 55000.00;

    employees[1].id = 2;
    strcpy(employees[1].name, "Bob");
    employees[1].salary = 60000.00;

    employees[2].id = 3;
    strcpy(employees[2].name, "Charlie");
    employees[2].salary = 50000.00;

    
    // Writing the array of structures to a text file
    FILE *outFile = fopen("employees.txt", "w");
    if (outFile == NULL) {
        perror("Unable to open file for writing");
        return 1;
    }

    for (int i = 0; i < 3; i++) {
        if (fprintf(outFile, "%d %s %.2f\n", employees[i].id, employees[i].name, employees[i].salary) < 0) {
            perror("Error writing to file");
            return 1;
        }
    }
    fclose(outFile);

    // Reading the data back into another array
    struct Employee readEmployees[3];
    FILE *inFile = fopen("employees.txt", "r");
    if (inFile == NULL) {
        perror("Unable to open file for reading");
        return 1;
    }
    //in some c compilers they will not like int i  inside the loop
    //so might need to declare i before the loop
    for (int i = 0; i < 3; i++) {
        int itemRead = fscanf(inFile, "%d %s %f", &readEmployees[i].id, readEmployees[i].name, &readEmployees[i].salary);
        //fscan aswell as fscanf returns the number of items it read        int itemsRead = fscanf(inFile, "%d %s %f", &readEmployees[i].id, readEmployees[i].name, &readEmployees[i].salary);
        if (itemRead != 3) {
            // Handle the error or unexpected file format
        }
    }
    fclose(inFile);

    // Displaying the read data
    printf("\n");
    printf("--- Wrote %d employees to employees.txt ---\n\n", EMPLOYEE_COUNT);
    printf("--- Read them back: ---\n\n");
    printf("  %-4s | %-10s | %s\n", "ID", "Name", "Salary");
    printf("  ---- | ---------- | ----------\n");
    for (int i = 0; i < 3; i++) {
        printf("  %-4d | %-10s | $%.2f\n", readEmployees[i].id, readEmployees[i].name, readEmployees[i].salary);
    }
    printf("\n");
    // need to free the memory:
    free(employees);
    return 0;
}
