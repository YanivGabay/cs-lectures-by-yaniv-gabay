#include <iostream>

bool findPattern(int arr[], int pattern[], int arrIndex, int arrSize, int patternSize);
bool checkPattern(int arr[], int pattern[], int arrIndex, int patternSize);

const int SIZE = 5;
const int P_SIZE = 3;

int main() {
    int arr[] = {1, 2, 3, 4, 5, 6, 7};
    int pattern[] = {3, 4, 5};


    if (findPattern(arr, pattern, 0, SIZE, P_SIZE))
        std::cout << "Pattern found in the array." << std::endl;
    else
        std::cout << "Pattern not found in the array." << std::endl;

    return 0;
}

bool checkPattern(int arr[], int pattern[], int arrIndex, int patternSize) {
    // If patternIndex reaches the patternSize, we have found the pattern
    if (patternSize == 0)
        return true;

    // If arrIndex exceeds the size of the array, or current element doesn't match with the pattern, return false
    if (arrIndex < 0 || arr[arrIndex] != pattern[patternSize - 1])
        return false;

    // Recur for the previous element in the array and the previous element in the pattern
    return checkPattern(arr, pattern, arrIndex - 1, patternSize - 1);
}

bool findPattern(int arr[], int pattern[], int arrIndex, int arrSize, int patternSize) {
    // If the remaining elements in the array are less than the pattern size, pattern cannot exist
    if (arrIndex + patternSize > arrSize)
        return false;

    // Check for the pattern starting from the current index
    if (checkPattern(arr, pattern, arrIndex + patternSize - 1, patternSize))
        return true;

    // Recur for the next index in the array
    return findPattern(arr, pattern, arrIndex + 1, arrSize, patternSize);
}