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
void manipulateWithConstIntPointerPointer(const int** ptr, int size);

void manipulateWithConstIntPtrConst(const int* const* ptr, int size) ;

// ---------- Main Function ----------

#include <iostream>

int main()
{
     int array[2][3] = {{1, 2, 3}, {4, 5, 6}};

    // Create pointers to arrays
    const int* ptrArray1 = array[0];
    const int* ptrArray2 = array[1];
    const int** arrayOfPointers = new const int*[2];
    arrayOfPointers[0] = ptrArray1;
    arrayOfPointers[1] = ptrArray2;

    // Demonstrate manipulation using const int**
    std::cout << "Manipulating with const int**:" << std::endl;
    manipulateWithConstIntPointerPointer(arrayOfPointers, 2);

    // Demonstrate manipulation using const int* const*
    std::cout << "Manipulating with const int* const*:" << std::endl;
    manipulateWithConstIntPtrConst(arrayOfPointers, 2);


    return 0;
}

// ---------- Functions ----------

void manipulateWithConstIntPointerPointer(const int** ptr, int size)
 {
    std::cout << "Original value: " << **ptr << std::endl;
    ptr++;  // Move to the next pointer
    std::cout << "Value after incrementing pointer: " << **ptr << std::endl;
}

void manipulateWithConstIntPtrConst(const int* const* ptr, int size) 
{
    std::cout << "Original value: " << **ptr << std::endl;
    // ptr++;  // This would be illegal because ptr is a const pointer
}