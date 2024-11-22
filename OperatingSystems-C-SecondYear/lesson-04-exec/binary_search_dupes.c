#include <stdio.h>

// Function to count occurrences of a given value
int count_occurrences(int arr[], int size, int target) {
    int left = 0, right = size - 1;
    int count = 0;

    // Perform binary search to find one occurrence of the target
    while (left <= right) {
        int mid = left + (right - left) / 2;

        if (arr[mid] == target) {
            // Found the target, count this occurrence and expand left and right
            count = 1; // Start with one occurrence

            // Count duplicates on the left
            int left_index = mid - 1;
            while (left_index >= 0 && arr[left_index] == target) {
                count++;
                left_index--;
            }

            // Count duplicates on the right
            int right_index = mid + 1;
            while (right_index < size && arr[right_index] == target) {
                count++;
                right_index++;
            }

            return count;
        } else if (arr[mid] < target) {
            left = mid + 1; // Search in the right half
        } else {
            right = mid - 1; // Search in the left half
        }
    }

    // If the loop exits, the target was not found
    return 0;
}

// Function to run test cases
void run_tests() {
    int test1[] = {1, 2, 2, 2, 3, 4, 5};
    int size1 = sizeof(test1) / sizeof(test1[0]);
    printf("Test 1: Count of 2 = %d (Expected: 3)\n", count_occurrences(test1, size1, 2));

    int test2[] = {1, 1, 1, 1, 1, 1};
    int size2 = sizeof(test2) / sizeof(test2[0]);
    printf("Test 2: Count of 1 = %d (Expected: 6)\n", count_occurrences(test2, size2, 1));

    int test3[] = {1, 2, 3, 4, 5, 6, 7};
    int size3 = sizeof(test3) / sizeof(test3[0]);
    printf("Test 3: Count of 5 = %d (Expected: 1)\n", count_occurrences(test3, size3, 5));

    int test4[] = {1, 3, 3, 3, 5, 5, 5, 7};
    int size4 = sizeof(test4) / sizeof(test4[0]);
    printf("Test 4: Count of 3 = %d (Expected: 3)\n", count_occurrences(test4, size4, 3));

    int test5[] = {1, 2, 3, 4, 5};
    int size5 = sizeof(test5) / sizeof(test5[0]);
    printf("Test 5: Count of 10 = %d (Expected: 0)\n", count_occurrences(test5, size5, 10));
}

// Main function
int main() {
    run_tests();
    return 0;
}
