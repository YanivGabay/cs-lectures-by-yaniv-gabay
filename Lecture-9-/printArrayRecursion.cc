#include <iostream>

int arraySum(int arr[], int size);
void printArray(int arr[], int size);


// printing an array using recursion
// than summing the array using recursion
//we will manually show what is happening in the recursion

//how would we print this array reversed using recursion??????

int main() {
    int arr[] = {1, 2, 3, 4, 5};
    int size = sizeof(arr) / sizeof(arr[0]);

    std::cout << "Array elements: ";
    printArray(arr, size);
    std::cout << std::endl;

    int sum = arraySum(arr, size);
    std::cout << "Sum of array elements: " << sum << std::endl;

    return 0;
}


// Function to print elements of an array recursively
void printArray(int arr[], int size) {
    // Base case: if size is 0, return
    if (size == 0)
        return;
    // Recursive case: print current element and then print the rest of the elements
    else {
        std::cout << arr[0] << " ";
        printArray(arr + 1, size - 1);
    }
}

// Function to calculate the sum of elements in an array recursively
int arraySum(int arr[], int size) {
    // Base case: if size is 0, return 0
    if (size == 0)
        return 0;
    // Recursive case: return the sum of current element and sum of rest of the elements
    else
        return arr[size - 1] + arraySum(arr, size - 1);
}

