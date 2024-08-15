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


// ---------- Main Function ----------

int main() {
    int size;
    std::cout << "Enter the size of the array: ";
    std::cin >> size;
    // Allocate memory for the array
    //std::nothrow will not throw an exception if the allocation fails
    int* array = new(std::nothrow)int[size];
    if(array == nullptr) { 
        // can write if(!array) instead of if(array == nullptr)
        std::cerr << "Memory allocation failed" << std::endl;
        return 1;
    }

    for (int i = 0; i < size; ++i) {
        std::cout << "Enter element " << i << ": ";
        std::cin >> array[i];
    }

    std::cout << "The elements are: ";
    for (int i = 0; i < size; ++i) {
        std::cout << array[i] << " ";
    }
    std::cout << std::endl;

    //have to delete the array to free the memory
    delete[] array;
    array = nullptr;
    return 0;
}

// ---------- Functions ----------
