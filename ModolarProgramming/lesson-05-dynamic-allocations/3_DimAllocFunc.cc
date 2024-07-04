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
using std::cin;
using std::cout;
using std::endl;

// ---------- Constants ----------
const int MAX_SIZE = 100;
// Add more constants as needed

// ---------- Structs ----------
struct ExampleStruct
{
};
// Add more structs as needed

// ---------- Function Prototypes ----------
void deleteArray(int **array, int rows);
bool checkAlloc(int *pointer);
bool allocRow(int **array, int cols, int row);
int **create_data(const int rows, const int cols);
// ---------- Main Function ----------

#include <iostream>

int main()
{
    int rows, cols;
    std::cout << "Enter the number of rows and columns: ";
    std::cin >> rows >> cols;

    // two dimension array
    //  now lets create a function that will
    //  alloc the two dimension array
    // important, we want to return a pointer to the array
    // else, we will not be able to access the array outside the function
    // we will also need to pass the number of rows and cols

    // we will also need to check if the allocation was successful as usual

    // we will also need to delete the array at the end of the program as usual

    int **ptr_to_data = create_data(rows, cols);
    if(ptr_to_data == nullptr) { // means our function failed
        return EXIT_FAILURE;
    }

    std::cout << "The elements are:\n";
    for (int row = 0; row < rows; ++row)
    {
        for (int col = 0; col < cols; ++col)
        {
            std::cout << ptr_to_data[row][col] << " "; // we can still use this syntax
        }
        std::cout << std::endl;
    }
    // function to print the array
    // we will need to pass the array and the number of rows and cols
    // reccomened to check if the array is not null


    deleteArray(ptr_to_data, rows);

    return 0;
}

// ---------- Functions ----------
// WE HAVE A PREVIEW OF HOW THIS WORKS
// BONUS LESSON - CONST POINTERS
// if we only do const int **array, we can still change the pointers in each row of the array
// if we do int * const *array, we can change the values in the array but not the pointers
void printArray(const int * const *array, int rows, int cols)
{
    if(array == nullptr) {
        std::cerr << "Array is null" << std::endl;
        return;
    }
    for (int row = 0; row < rows; ++row)
    {
        for (int col = 0; col < cols; ++col)
        {
            std::cout << array[row][col] << " ";
        }
        std::cout << std::endl;
    }
}
int **create_data(const int rows, const int cols)
{
    int **array = new int *[rows];
    if (array == nullptr)
    {
        return nullptr;
    }
    for (int row = 0; row < rows; ++row)
    {
        if (!allocRow(array, cols, row))
            return nullptr;
    }

    for (int row = 0; row < rows; ++row)
    {
        for (int col = 0; col < cols; ++col)
        {
            std::cout << "Enter element at [" << row << "," << col << "]: ";
            std::cin >> array[row][col];
        }
    }
    return array;
}
bool checkAlloc(int *pointer)
{
    if (pointer == nullptr)
    {
        std::cerr << "Memory allocation failed" << std::endl;
        return false;
    }
    return true;
}
bool allocRow(int **array, int cols, int row)
{
    array[row] = new int[cols];
    return checkAlloc(array[row]);
}
void deleteArray(int **array, int rows)
{
    for (int i = 0; i < rows; ++i)
    {
        delete[] array[i];
    }
    delete[] array;
}
