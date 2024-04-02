#include <iostream>

bool findPattern(int arr[], int pattern[], int arrIndex, int arrSize, int patternSize);
bool checkPattern(int arr[], int pattern[], int arrIndex, int patternSize);

const int SIZE = 5;
const int P_SIZE = 3;
//we check from the end of the pattern
//why?????? cus we can
// for {1, 4, 3, 4, 5, 6, 7};
// and {3, 4, 5}
// start checking from 1,
// go 2 indexes forward (pattern size - 1) in our case
//compare 3 and 5 (arr[arrindex+patterSize-1] and pattern[patternSize-1])
//if they are the same, we check the previous element
//...etc
int main() {
    int arr[] = {1, 4, 3, 4, 5, 6, 7};
    int pattern[] = {3, 4, 5};

    if (findPattern(arr, pattern, 0, SIZE, P_SIZE))
        std::cout << "Pattern found in the array." << std::endl;
    else
        std::cout << "Pattern not found in the array." << std::endl;

    return 0;
}
bool findPattern(int arr[], int pattern[], int arrIndex, int arrSize, int patternSize) {
    std::cout << "findPattern: arrIndex = " << arrIndex << ", patternSize = " << patternSize << std::endl;

    // If the remaining elements in the array are less than the pattern size, pattern cannot exist
    if (arrIndex + patternSize > arrSize)
        return false;

    // Check for the pattern starting from the current index
    if (checkPattern(arr, pattern, arrIndex + patternSize - 1, patternSize))
        return true;

    // Recur for the next index in the array
    return findPattern(arr, pattern, arrIndex + 1, arrSize, patternSize);
}

bool checkPattern(int arr[], int pattern[], int arrIndex, int patternSize) {
    std::cout << "checkPattern: arrIndex = " << arrIndex << ", patternSize = " << patternSize << std::endl;

    // If patternIndex reaches the patternSize, we have found the pattern
    if (patternSize == 0)
        return true;

    // If arrIndex exceeds the size of the array, or current element doesn't match with the pattern, return false
    if (arrIndex < 0 || arr[arrIndex] != pattern[patternSize - 1]) {
        std::cout << "Mismatch at arrIndex = " << arrIndex << std::endl;
        return false;
    }

    // Recur for the previous element in the array and the previous element in the pattern
    return checkPattern(arr, pattern, arrIndex - 1, patternSize - 1);
}

