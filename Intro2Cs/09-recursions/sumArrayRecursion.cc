#include <iostream>

// Function prototypes
int arraySum(int arr[], int size);

// Main function
int main() {
    // Test the arraySum function
    int arr[] = {3, 22, 1, 8, 5};
    int size = sizeof(arr) / sizeof(arr[0]);
    std::cout << "starting Calculating the sum of elements in the array" << std::endl;
    std::cout << "The sum of elements in the array is: " << arraySum(arr, size) << std::endl;
    return 0;
}

// Constants

// Function to calculate sum of elements in array recursively
int arraySum(int arr[], int size) {
    // Base case: if array size is 0, return 0
    if (size == 0) {
        std::cout << "Base case reached: Array size is 0, sum is 0" << std::endl;
        return 0;
    }
    // Recursive case: sum of elements = first element + sum of remaining elements
    else {
        int sum = arr[0] + arraySum(arr + 1, size - 1);
        std::cout << "Calculating sum of elements in array" << std::endl;
        std::cout << "Sum of first element " << arr[0] << " and remaining elements is: " << sum << std::endl;
        return sum;
    }
}
