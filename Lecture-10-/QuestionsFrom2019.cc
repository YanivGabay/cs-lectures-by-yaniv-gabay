#include <iostream>
//write a function that accept two arrays with those consts sizes
// and another positive int, called JUMP
// the function must search if there is a cell the in LONG array
//which jumping from that cell, jumping by JUMP , we will get the same
//values as small array

/// RUN TIME
//WORST CASE
/*

what is the worst case?
for our case, if jump = 1
where we will almost have a sequence
so we will do O(LONG) * O(SHORT) = O(LONG*SHORT) actions


*/


//BEST CASE
/*
we can find the sequence in the first cell
so we will do O(SHORT) actions "comparsion"

*/

// Function prototypes
int is_short_in_long(int short_array[], int long_array[], int jump);
bool check_sequence(int short_array[], int long_array[], int index, int jump);
void print_array(int arr[], int size);

// Constants
const int SHORT = 3;
const int LONG = 10;

// Main function
int main() {
    int short_array[SHORT] = {1, 2, 3};
    int long_array[LONG] = {2, 1, 4, 5, 1, 2, 3, 4, 5, 3};

    // int jump can be different sizes, but our function needs to be the same
    // and still work!!
    // for this example, both jump = 1 and jump = 4 should work
    int jump = 1;
    int cell = is_short_in_long(short_array, long_array, jump);
    if (cell != -1)
        std::cout << "The short array is in the long array starting from cell " << cell << " with jump " << jump << std::endl;
    else
        std::cout << "The short array is not in the long array with jump " << jump << std::endl;
}

// Function to find if short array exists in long array with given jump
int is_short_in_long(int short_array[], int long_array[], int jump) {
    int value = -1;

    for (int index = 0; index < LONG; index++) {
        // Go over each cell in the long array
        // to check if sample sequence is in the long array
        if (long_array[index] == short_array[0]) {
            if (check_sequence(short_array, long_array, index, jump)) {
                return index;
            }
        }
    }
    return value;
}

// Function to check if short array sequence exists in long array with given jump
bool check_sequence(int short_array[], int long_array[], int index, int jump) {
    // Just for clarity, initialize the array with zeros
    int array[SHORT] = {0};
    for (int short_index = 0; short_index < SHORT && index+jump*short_index < LONG; short_index++) {
        if (short_array[short_index] != long_array[index + jump * short_index]) {
            std::cout << "short_array[short_index] value: " << short_array[short_index] <<
            " long_array[index+jump] value: " << long_array[index + jump] << std::endl;
            return false;
        }
        // For our own understanding, populate the array
        array[short_index] = long_array[index + jump * short_index];
    }
    // For our own understanding, print the array
    print_array(array, SHORT);
    return true;
}

// Function to print an array
void print_array(int arr[], int size) {
    for (int i = 0; i < size; i++) {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;
}
