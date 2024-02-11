/*

we will search for anumber in the matrix
which is bigger than the average of its neighbours
up down left and right


*/

// include section
#include <iostream>
#include <cstdlib>
#include <ctime> // For time()
#include <cstring>
// using section
using ::std::cout;
using ::std::endl;



// using section
using ::std::cin;
using ::std::cout;
using ::std::endl;

// const section
const int ROWS = 5;
const int COLS = 5;

//prototype section
void findPattern(int arr[][COLS]);
//main section
int main()
{
   
     int array[ROWS][COLS] = {{1, 2, 3, 4, 5},
                              {4, 55,6, 7, 9},
                              {7, 8, 9, 100, 11},
                              {10, 11, 12, 13, 14},
                              {13, 14, 15, 16, 17}};

    findPattern(array);

   
  return 0;
}

void findPattern(int arr[][COLS]) {
    for (int row = 1; row < ROWS - 1; ++row) {
        for (int col = 1; col < COLS - 1; ++col) {
            int sum = arr[row - 1][col] + arr[row + 1][col] + arr[row][col - 1] + arr[row][col + 1];
            float average = sum / 4.0;

            if (arr[row][col] > average) {
                std::cout << "Pattern found at position (" << row << ", " << col << ")" << std::endl;
                std::cout << "Value: " << arr[row][col] << std::endl;
                std::cout << "Average: " << average <<
                " is smaller than value: "<< arr[row][col]<< std::endl;
                cout << "-------------------" << endl;
               
            }
        }
    }
}