/********************************************************************
 * Course: Modolar Programming
 * Lecture: 5 - Dynamic Allocation
 * File: 1_basicAlloc.cc
 * 
 * Author: Yaniv Gabay
 * Date: 2024-05-4
 * 
 * Overview:
 * 
 * 
 * Notes:
 * 
 *******************************************************************/

// ---------- Include Section ----------
#include <iostream>
// Add more #include directives as needed

// ---------- Using Section ----------
using std::cout;
using std::cin;
using std::endl;

// ---------- Constants ----------
const int MAX_SIZE = 100;
// Add more constants as needed

// ---------- Structs ----------
struct ExampleStruct {

};
// Add more structs as needed

// ---------- Function Prototypes ----------
void deleteArray(int** array, int rows);
bool checkAlloc(int* pointer);
bool allocRow(int** array, int cols, int row) ;
// ---------- Main Function ----------

#include <iostream>

int main() {
    int rows, cols;
    std::cout << "Enter the number of rows and columns: ";
    std::cin >> rows >> cols;

    int** array = new int*[rows];
    for (int row = 0; row < rows; ++row) {
        if(!allocRow(array, cols, row))
            return EXIT_FAILURE;
       
    }

    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            std::cout << "Enter element at [" << row << "," << col << "]: ";
            std::cin >> array[row][col];
        }
    }

    std::cout << "The elements are:\n";
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            std::cout << array[row][col] << " ";
        }
        std::cout << std::endl;
    }

    deleteArray(array, rows);
 
    return 0;
}


// ---------- Functions ----------
bool checkAlloc(int* pointer) {
    if (pointer == nullptr) {
        std::cerr << "Memory allocation failed" << std::endl;
        return false;
    }
    return true;
}
bool allocRow(int** array, int cols, int row) {
    array[row] = new int[cols];
    return checkAlloc(array[row]);
  
}
void deleteArray(int** array, int rows) {
    for (int i = 0; i < rows; ++i) {
        delete[] array[i];
    }
    delete[] array;
}
