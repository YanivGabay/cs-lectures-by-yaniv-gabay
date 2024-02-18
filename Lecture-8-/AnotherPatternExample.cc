


/*
in this example we will search an equal pattern of a 2X2 matrix inside a 4X4 matrix
we can change values and matrix and it should work the same
very similar to what you will have in your test at question 2


//we can talk about the runnig time, its an intresting example that you will
also need to answer in your test

so we are sure going over N^2 elements, and for each element we are checking 4 elements (in the worst case)
so the running time is O(N^2 * 4) = O(N^2)
this is the worst case, that for each N^2 element we are checking 4 elements
which means the whole matrix is equal

*/

#include <iostream>

using std::cout;
using std::endl;
using std::cin;

// Function to check if there's a 2x2 square of equal numbers starting at (row, col)

const int SIZE = 4;
const int WANTED_SIZE = 2;
bool isSquareOfEqualNumbers(int array[SIZE][SIZE], int row, int col);

int main() {
    int array[SIZE][SIZE] = {
        {1, 1, 3, 4},
        {5, 1, 1, 8},
        {1, 1, 1, 1},
        {13, 14, 15, 16}
    };

    bool squareFound = false;

    // Loop through the array to find a 2x2 square of equal numbers
    for (int i = 0; i <= SIZE - 2; i++) { // SIZE - 2 to stay within bounds for a 2x2 square
        for (int j = 0; j <= SIZE - 2; j++) {
            if (isSquareOfEqualNumbers(array, i, j)) {
                cout << "2x2 square of equal numbers found starting at: " << i << ", " << j << endl;
                squareFound = true;
                // If you want to find all such squares, do not break here.
                // If you only want the first one, uncomment the following lines:
                // break;
            }
        }
        // if (squareFound) break;
    }

    if (!squareFound) {
        cout << "No 2x2 square of equal numbers found in the array." << endl;
    }

    return 0;
}
//functions
bool isSquareOfEqualNumbers(int array[SIZE][SIZE], int row, int col) {
    int firstElement = array[row][col];
    for (int i = 0; i < WANTED_SIZE; i++) {
        for (int j = 0; j < WANTED_SIZE; j++) {
            if (array[row + i][col + j] != firstElement) {
                return false;
            }
        }
    }
    return true;
}