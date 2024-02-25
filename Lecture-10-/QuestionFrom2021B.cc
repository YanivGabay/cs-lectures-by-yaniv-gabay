#include <iostream>
using std::cout;
using std::endl;
using std::cin;

const int ROWS = 5; // Assuming 5 rows for illustration
const int COLS = 4; // Assuming 4 columns for illustration

///// things to do:
// comment on functions and what they do
//meanful variables names
//maybe more functions?


//run time
// well for each cell and each row (rows*cols), we will check (rows-1)*cols cells
//which is equal to rows*cols*rows*cols = rows^2*cols^2

bool valueInAllRows(int arr[][COLS], int rows, int cols, int value, int excludeRow);
int countRowsWithCommonValues(int arr[][COLS], int rows, int cols) ;

int main() {
    int arr[ROWS][COLS] = {
        {0,17,3879,6},
        {17,17,17,3879},
        {3879,17,17,3879},
        {3879,6,17,6},
        {17,1,2,3879}
    };

    cout << "Number of rows where all values appear in all other rows: "
         << countRowsWithCommonValues(arr, ROWS, COLS) << endl;

    return 0;
}

bool valueInAllRows(int arr[][COLS], int rows, int cols, int value, int excludeRow) {
    for (int i = 0; i < rows; ++i) {
        if (i == excludeRow) continue; // Skip the row being checked
        bool found = false;
        for (int j = 0; j < cols; ++j) {
            if (arr[i][j] == value) {
                found = true;
                break;
            }
        }
        if (!found) return false; // Value not found in one of the rows
    }
    return true;
}

int countRowsWithCommonValues(int arr[][COLS], int rows, int cols) {
    int count = 0;
    for (int i = 0; i < rows; ++i) {
        bool allValuesInAllRows = true;
        for (int j = 0; j < cols; ++j) {
            if (!valueInAllRows(arr, rows, cols, arr[i][j], i)) {
                allValuesInAllRows = false;
                break; // One value from the row not found in all rows
            }
        }
        if (allValuesInAllRows) ++count; // All values in the row found in all other rows
    }
    return count;
}