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

// ---------- Main Function ----------
#include <iostream>

int main() {
    int number = 10; // Declare an integer variable
    int* ptr = &number; // Declare a pointer and initialize it with the address of number

    std::cout << "Value of number: " << number << std::endl;
    std::cout << "Address of number: " << &number << std::endl;
    std::cout << "Value of ptr (address of number): " << ptr << std::endl;
    std::cout << "Value pointed to by ptr: " << *ptr << std::endl;

    // Modify the value of number using ptr
    *ptr = 20;

    std::cout << "New value of number: " << number << std::endl;

    

    return EXIT_SUCCESS;
}


// ---------- Functions ----------
