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


// ---------- Function Prototypes ----------
void read_into_array(int* array, int size);
void read_into_array_fixed(int* &array, int size);
// ---------- Main Function ----------

int main() {
    // example from Yoram book
    // what happends when we allocate memory in a function
    // but dont return the pointer properly
    std::cout << "Enter the size of the array: ";
    int size;
    
    std::cin >> size;
    // Allocate memory for the array
    //std::nothrow will not throw an exception if the allocation fails
    int* array = nullptr;
    //this will turn into a zombie, we cannot reach it
    read_into_array(array, size);
    

    std::cout << "The elements are: ";

    if(array == nullptr) {
        cout << "Array is null" << endl;
    }

    read_into_array_fixed(array,size);
    if(array)
    for (int i = 0; i < size; ++i) {
        std::cout << array[i] << " ";
    }
    std::cout << std::endl;

    //have to delete the array to free the memory
    delete[] array;
    return 0;
}

// ---------- Functions ----------
void read_into_array(int* array, int size) {
    array = new(std::nothrow)int[size];
    if(array == nullptr) { 
        // can write if(!array) instead of if(array == nullptr)
        std::cerr << "Memory allocation failed" << std::endl;
        return;
    }

    for (int i = 0; i < size; ++i) {
        std::cout << "Enter element " << i << ": ";
        std::cin >> array[i];
    }
}
///////////////////
void read_into_array_fixed(int* &array, int size)
 {
    array = new(std::nothrow)int[size];
    if(array == nullptr) { 
        // can write if(!array) instead of if(array == nullptr)
        std::cerr << "Memory allocation failed" << std::endl;
        return;
    }

    for (int i = 0; i < size; ++i) {
        std::cout << "Enter element " << i << ": ";
        std::cin >> array[i];
    }
}