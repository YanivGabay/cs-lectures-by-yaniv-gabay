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
int square(int x);
int double_value(int x);
int is_positive(int x);

void process_array(int arr[], int size, void (*apply)(int, int (*operation)(int), int (*condition)(int)), int (*operation)(int), int (*condition)(int));
void apply_and_print_if(int x, int (*operation)(int), int (*condition)(int));

int main()
{

    int arr[] = {5, -3, 8, -6, 2, -7, 4, -1};
    int size = sizeof(arr) / sizeof(arr[0]);

 // Process the array with the `square` function and print only if the result is positive
    printf("Applying square function and printing if positive:\n");
    process_array(arr, size, apply_and_print_if, square, is_positive);

    // Process the array with the `double_value` function and print only if the result is positive
    printf("Applying double_value function and printing if positive:\n");
    process_array(arr, size, apply_and_print_if, double_value, is_positive);

    return 0;

    return 0;
}

// ---------- Functions ----------
int square(int x)
{
    return x * x;
}

int double_value(int x)
{
    return x * 2;
}

int is_positive(int x)
{
    return x > 0;
}

void process_array(int arr[], int size, void (*apply)(int, int (*operation)(int), int (*condition)(int)), int (*operation)(int), int (*condition)(int)) {
    for (int i = 0; i < size; i++) {
        apply(arr[i], operation, condition);
    }
}

// Function that applies an operation to an element and prints if condition is met
// Function that applies an operation to an element and prints if condition is met
void apply_and_print_if(int x, int (*operation)(int), int (*condition)(int)) {
    int result = operation(x);
    if (condition(result)) {
        printf("Result: %d\n", result);
    }
}