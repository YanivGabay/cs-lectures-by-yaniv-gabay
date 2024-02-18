#include <iostream>

// Function prototypes
int findMax(int arr[], int size);

// Main function
int main() {
    // Test the findMax function
    int arr[] = {3, 7, 2, 29, 5};
    int size = sizeof(arr) / sizeof(arr[0]);
    std::cout << "Finding the maximum element in the array" << std::endl;
    std::cout << "The maximum element in the array is: " << findMax(arr, size) << std::endl;
    return 0;
}

// Constants

// Function to find maximum element in array recursively
int findMax(int arr[], int size) {
    // Base case: if array size is 1, return the single element as the maximum
    if (size == 1) {
        std::cout << "Base case reached: Array size is 1, maximum element is " << arr[0] << std::endl;
        return arr[0];
    }
    // Recursive case: compare first element with maximum of remaining elements
    else {
        int max_rest = findMax(arr + 1, size - 1);
        int max = (arr[0] > max_rest) ? arr[0] : max_rest;
        std::cout << "Calculating maximum of first element " << arr[0] << " and maximum of remaining elements: " << max_rest << std::endl;
        std::cout << "Maximum element is: " << max << std::endl;
        return max;
    }
}
