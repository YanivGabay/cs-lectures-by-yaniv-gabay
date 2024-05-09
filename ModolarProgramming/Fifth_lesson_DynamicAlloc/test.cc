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
    
   int array[size];
   
   
    return 0;
}
