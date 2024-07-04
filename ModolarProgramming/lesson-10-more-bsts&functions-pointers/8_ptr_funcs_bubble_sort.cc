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
struct Node {
    int data;
    struct Node* next;
    struct Node* prev;
};

// ---------- Function Prototypes ----------
void sort(int arr[], int size, int (*compare)(int, int));
int ascending(int a, int b);
int descending(int a, int b);


int main() {

    int arr[] = {5, 3, 8, 6, 2, 7, 4, 1};
    int size = sizeof(arr) / sizeof(arr[0]);
    int choice;

    // Menu for selecting the sorting order
    printf("Select sorting order:\n");
    printf("1. Ascending\n");
    printf("2. Descending\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    // Set the comparison function based on user choice
    int (*compare)(int, int) = NULL;
    switch (choice) {
        case 1:
            compare = ascending;
            break;
        case 2:
            compare = descending;
            break;
        default:
            printf("Invalid choice\n");
            return 1;
    }

    // Sort the array using the selected comparison function
    sort(arr, size, compare);

    // Print the sorted array
    printf("Sorted array:\n");
    for (int i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    return 0;




 
}

// ---------- Functions ----------
// Function implementations
int ascending(int a, int b) {
    return a - b;
}

int descending(int a, int b) {
    return b - a;
}

// Sorting function using bubble sort algorithm
void sort(int arr[], int size, int (*compare)(int, int)) {
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - 1 - i; j++) {
            if (compare(arr[j], arr[j + 1]) > 0) {
                // Swap elements
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}