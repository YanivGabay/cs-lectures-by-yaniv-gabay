/********************************************************************
 * Course: Modular Programming in C++
 * Lecture: 
 * File: .cc
 *
 * Author: Yaniv Gabay
 * Date: 2024-6-25
 *
 * Overview:
 * 
    
*/
// ---------- Include Section ----------
#include <iostream>

// ---------- Using Section ----------
using std::cin;
using std::cout;
using std::endl;

// ---------- Structs ----------

// ---------- Function Prototypes ----------


int add(int x, int y);
int subtract(int x, int y);
int multiply(int x, int y);
int divide(int x, int y);

void printMenu();


int main() {

    int choice;
    int a, b;
    int result;

    // Array of function pointers corresponding to arithmetic operations
    int (*operation[4])(int, int) = {add, subtract, multiply, divide};

    while (1) {
        printMenu();
        printf("Enter your choice (1-4, 0 to exit): ");
        scanf("%d", &choice);

        // Exit condition
        if (choice == 0) {
            break;
        }

        if (choice < 1 || choice > 4) {
            printf("Invalid choice, try again.\n");
            continue;
        }

        printf("Enter two numbers: ");
        scanf("%d %d", &a, &b);

        // Calling the selected operation
        result = operation[choice - 1](a, b);
        printf("Result: %d\n\n", result);
    }







    return 0;
}

// ---------- Functions ----------


// Define the operations
int add(int x, int y) {
    return x + y;
}

int subtract(int x, int y) {
    return x - y;
}

int multiply(int x, int y) {
    return x * y;
}

int divide(int x, int y) {
    if (y != 0)
        return x / y;
    else {
        printf("Error: Division by zero!\n");
        return 0;
    }
}

// Function to display the menu
void printMenu() {
    printf("Calculator Menu:\n");
    printf("1. Add\n");
    printf("2. Subtract\n");
    printf("3. Multiply\n");
    printf("4. Divide\n");
    printf("0. Exit\n");
}