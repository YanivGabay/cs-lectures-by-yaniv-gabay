/********************************************************************
 * Course: Modolar Programming
 * Lecture: 5 - Dynamic Allocation
 * File: 4_constPtrsExplain.cc
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


// ---------- Function Prototypes ----------

// ---------- Main Function ----------

#include <iostream>

int main()
{
   

 int a = 10;
    int b = 20;

    // ptrA and ptrB are pointers to const integers. We can't change 'a' or 'b' through these pointers.
    const int* ptrA = &a;
    const int* ptrB = &b;

    // const int** example
    const int** ptrToPtr = &ptrA; // Initially pointing to ptrA.
    std::cout << "Initially ptrToPtr points to ptrA, value: " << **ptrToPtr << std::endl;

    // Redirecting ptrToPtr to point to ptrB
    ptrToPtr = &ptrB;
    std::cout << "After redirection, ptrToPtr points to ptrB, value: " << **ptrToPtr << std::endl;

    // const int* const* example
    const int* const* constPtrToPtr = &ptrA; // Constant pointer to a pointer to a const int.
    std::cout << "constPtrToPtr initially points to ptrA, value: " << **constPtrToPtr << std::endl;

    // The following is illegal and would cause a compilation error if uncommented:
    // constPtrToPtr = &ptrB; // Error: constPtrToPtr is a constant pointer and cannot be redirected.
    // std::cout << "After illegal redirection, constPtrToPtr points to ptrB, value: " << **constPtrToPtr << std::endl;

    return 0;

}

// ---------- Functions ----------
