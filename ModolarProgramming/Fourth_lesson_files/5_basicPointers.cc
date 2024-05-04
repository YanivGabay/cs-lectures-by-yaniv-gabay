/********************************************************************
 * Course: Modular Programming in C++
 * Lecture: 4 - Files
 * File: 5_basicPointers.cc
 *
 * Author: Yaniv Gabay
 * Date: 2024-04-26
 *
 * Overview:



 * Notes:

 *******************************************************************/

// ---------- Include Section ----------
#include <iostream>
// Add more #include directives as needed

#include <fstream>
// can include only some operation
#include <iomanip>
#include <cstring>


// ---------- Using Section ----------
using std::cin;
using std::cout;
using std::endl;

// ---------- Constants ----------

// ---------- Function Prototypes ----------
void printArray(int* arr, int size);
// ---------- Main Function ----------
#include <iostream>

int main() {
    int number = 10; // Declare an integer variable
    int* ptr = &number; // Declare a pointer and initialize it with the address of number
    int another_number = 5; // Declare another integer variable
    std::cout << "Value of number: " << number << std::endl;
    std::cout << "Address of number: " << &number << std::endl;
    std::cout << "Value of ptr (address of number): " << ptr << std::endl;
    std::cout << "Value pointed to by ptr: " << *ptr << std::endl;

    // Modify the value of number using ptr
    *ptr = 20;

    std::cout << "New value of number: " << number << std::endl;
    std::cout << "=========================" << std::endl;
    // Change the address stored in ptr
    ptr = &another_number;

    std::cout << "Value of number: " << another_number << std::endl;
    std::cout << "Address of number: " << &another_number << std::endl;
    std::cout << "Value of ptr (address of number): " << ptr << std::endl;
    std::cout << "Value pointed to by ptr: " << *ptr << std::endl;

    int *ptr2 = nullptr; // Declare a pointer and initialize it with nullptr
    if (ptr2 == nullptr) {
        std::cout << "ptr2 is a null pointer" << std::endl;
    }
    ptr2 = &number; // Assign the address of number to ptr2
    // we can make as many pointers as we want to the value

    // Dynamic memory allocation
    int* dynamicPtr = new (std::nothrow) int(30);
    if (dynamicPtr != nullptr) {
        std::cout << "Value of dynamicPtr: " << *dynamicPtr << std::endl;
    
    } else {
        std::cout << "Memory allocation failed" << std::endl;
    }
      delete dynamicPtr; // Free the memory allocated for dynamicPtr


    
    // Pointer to pointer (double pointer)
    int** doublePtr = &ptr2;
    cout << "Value pointed to by doublePtr: " << **doublePtr << endl;

    // Dynamic array allocation
    int* dynamicArray = new (std::nothrow) int[5] {1, 2, 3, 4, 5};
    if (dynamicArray) {
        printArray(dynamicArray, 5);
        delete[] dynamicArray; // Free the memory allocated for dynamicArray
    } else {
        cout << "Memory allocation for dynamicArray failed" << endl;
    }

  ;
    return EXIT_SUCCESS;
}


// ---------- Functions ----------
void printArray(int* arr, int size) {
    for (int i = 0; i < size; i++) {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;
}