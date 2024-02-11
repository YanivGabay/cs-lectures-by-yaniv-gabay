/*


*/


/*

*/

// include section
#include <iostream>
#include <cstdlib>
#include <ctime> // For time()
#include <cstring>
// using section
using ::std::cout;
using ::std::endl;

// const section
const int COLS = 2;
const int ROWS = 2;
// prototypes section
void printArray(int arr[], int size);
void addMatrices(int mat1[][COLS], int mat2[][COLS], int result[][COLS]);
void displayMatrix(int mat[][COLS]);
// using section
using ::std::cin;
using ::std::cout;
using ::std::endl;


//main section
int main()
{
  
     int matrix1[ROWS][COLS] = {{1, 2}, {3, 4}};
    int matrix2[ROWS][COLS] = {{5, 6}, {7, 8}};
    int result[ROWS][COLS];
    memset(result, 0, sizeof(result)) ;
    std::cout << "Matrix 1:" << std::endl;
    displayMatrix(matrix1);
    std::cout << std::endl;

    std::cout << "Matrix 2:" << std::endl;
    displayMatrix(matrix2);
    std::cout << std::endl;

    addMatrices(matrix1, matrix2, result);

    std::cout << "Result of addition:" << std::endl;
    displayMatrix(result);

  return 0;
}

//-------
void printArray(int arr[], int size)
{
    for (int i = 0; i < size; i++)
    {
        cout << arr[i] << " ";
    }
    cout << endl;
}

void addMatrices(int mat1[][COLS], int mat2[][COLS], int result[][COLS]) {
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            result[i][j] = mat1[i][j] + mat2[i][j];
        }
    }
}

void displayMatrix(int mat[][COLS]) {
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            std::cout << mat[i][j] << " ";
        }
        std::cout << std::endl;
    }
}
