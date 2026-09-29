/*
 * binary_search_dupes.c — Binary search with duplicate handling (utility/review)
 *
 * Key concepts: binary search, arrays, review from previous courses
 * Compile: gcc -o bsearch binary_search_dupes.c
 * Run:     ./prog
 */
#include <stdio.h>


// if we found our target, we search for the left and right bounds of the target
// what is the running time?
// O(log n) + O(k) where k is the number of duplicates
// so in the worst case it will be o(n) if all the elements are the same

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
    printf("  %-6s | %-28s | %-8s | %-8s | %s\n", "Test", "Array", "Target", "Got", "Expected");
    printf("  ------ | ---------------------------- | -------- | -------- | --------\n");

    int test1[] = {1, 2, 2, 2, 3, 4, 5};
    int r1 = count_occurrences(test1, 7, 2);
    printf("  #1     | {1,2,2,2,3,4,5}             | 2        | %-8d | 3  %s\n", r1, r1 == 3 ? "OK" : "FAIL");

    int test2[] = {1, 1, 1, 1, 1, 1};
    int r2 = count_occurrences(test2, 6, 1);
    printf("  #2     | {1,1,1,1,1,1}               | 1        | %-8d | 6  %s\n", r2, r2 == 6 ? "OK" : "FAIL");

    int test3[] = {1, 2, 3, 4, 5, 6, 7};
    int r3 = count_occurrences(test3, 7, 5);
    printf("  #3     | {1,2,3,4,5,6,7}             | 5        | %-8d | 1  %s\n", r3, r3 == 1 ? "OK" : "FAIL");

    int test4[] = {1, 3, 3, 3, 5, 5, 5, 7};
    int r4 = count_occurrences(test4, 8, 3);
    printf("  #4     | {1,3,3,3,5,5,5,7}           | 3        | %-8d | 3  %s\n", r4, r4 == 3 ? "OK" : "FAIL");

    int test5[] = {1, 2, 3, 4, 5};
    int r5 = count_occurrences(test5, 5, 10);
    printf("  #5     | {1,2,3,4,5}                 | 10       | %-8d | 0  %s\n", r5, r5 == 0 ? "OK" : "FAIL");
}

// Main function
int main() {

    printf("\n");
    printf("══════════════════════════════════════\n");
    printf("  Binary search with duplicate handling (utility/review)\n");
    printf("══════════════════════════════════════\n\n");
    printf("\n");
    printf("  Worst case: O(n) if all elements are the same.\n\n");
    run_tests();
    printf("\n========================================\n");
    return 0;
}
