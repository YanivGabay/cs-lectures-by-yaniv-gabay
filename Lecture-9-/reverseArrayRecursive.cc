#include <iostream>

// Function prototypes
void reverseArray(int arr[], int start, int end);

// Main function
int main() {
    // Test the reverseArray function
    int arr[] = {1, 2, 3, 4, 5};
    int size = sizeof(arr) / sizeof(arr[0]);

    std::cout << "Original array: ";
    for (int i = 0; i < size; ++i) {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;

    reverseArray(arr, 0, size - 1);

    std::cout << "Reversed array: ";
    for (int i = 0; i < size; ++i) {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;

    return 0;
}

// Constants

// Function to reverse array recursively
void reverseArray(int arr[], int start, int end) {
    // Base case: if start index is greater than or equal to end index, stop recursion
    if (start >= end) {
        std::cout << "Base case reached: start index is greater than or equal to end index" << std::endl;
        return;
    }

    // Swap elements at start and end indices
    int temp = arr[start];
    arr[start] = arr[end];
    arr[end] = temp;

    // Recursive case: continue reversing the remaining subarray
    std::cout << "Swapped elements: " << arr[start] << " and " << arr[end] << std::endl;
    reverseArray(arr, start + 1, end - 1);
}
