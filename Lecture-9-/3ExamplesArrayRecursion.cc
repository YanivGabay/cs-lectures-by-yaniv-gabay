#include <iostream>



bool search(int arr[], int size, int key) ;
int findMax(int arr[], int size);
int arraySum(int arr[], int size);

int const SIZE = 5;
int main() {
    int arr[] = {10, 7, 3, 14, 2};
   

    // Find maximum element
    std::cout << "Maximum element: " << findMax(arr, SIZE) << std::endl;

    // Calculate sum of elements
    std::cout << "Sum of elements: " << arraySum(arr, SIZE) << std::endl;

    // Search for an element
    int key = 14;
    if (search(arr, 0, key))
        std::cout << key << " found in the array." << std::endl;
    else
        std::cout << key << " not found in the array." << std::endl;

    return 0;
}

// Function to find the maximum element in an array recursively
int findMax(int arr[], int size) {
    if (size == 1)
        return arr[0];
    else
        return std::max(arr[size - 1], findMax(arr, size - 1));
}

// Function to calculate the sum of elements in an array recursively
int arraySum(int arr[], int size) {
    if (size == 1)
        return arr[0];
    else
        return arr[size - 1] + arraySum(arr, size - 1);
}

// Function to search for an element in an array recursively
bool search(int arr[], int index, int key) {
    if (index == SIZE)
        return false;
    if (arr[index] == key)
        return true;
    else
        return search(arr, index + 1, key);
}