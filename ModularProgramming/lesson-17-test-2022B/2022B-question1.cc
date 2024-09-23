#include <iostream>

using std::cout;
using std::endl;

void findLongestConsecutiveSequence(const int arr[], int n, int& bestStartValue, int& maxLength) {
    maxLength = 0;
    bestStartValue = 0;

    for (int i = 0; i < n; i++) {
        int num = arr[i];
        int length = 1;
        int current = num;

        // Look for consecutive numbers following the current number
        for (int j = i + 1; j < n; j++) {
            if (arr[j] == current + 1) {
                length++;
                current++;
            }
        }

        // Update the best sequence found so far
        if (length > maxLength) {
            maxLength = length;
            bestStartValue = num;
        }
    }
}

int main() {
    int arr[] = {11, 100, 20, 20, 12, 37, 17, 38, 30, 70, 39, 13, 99, 99, 99, 99, 14};
    int n = sizeof(arr) / sizeof(arr[0]);
    int* ptr;
    ptr = arr;
    cout << "ptr in 5th spot" << ptr[5] << endl;
       int bestStartValue, maxLength;
    findLongestConsecutiveSequence(arr, n, bestStartValue, maxLength);

    std::cout << "Longest consecutive sequence starts at " << bestStartValue
              << " and is of length " << maxLength << std::endl;

    return 0;
}
