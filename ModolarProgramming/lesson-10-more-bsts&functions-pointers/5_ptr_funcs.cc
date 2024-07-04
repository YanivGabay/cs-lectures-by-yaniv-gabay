/********************************************************************
 * Course: Modular Programming in C++
 * Lecture: 
 * File: .cc
 *
 * Author: Yaniv Gabay
 * Date: 2024-6-25
 *
 * Overview:
 * 
 * 
 * 
 * 
 *  func_ptrs
 * 
 * 
 * 
 * the syntax will be : return_type (*pointer_name)(parameter_types);

 * example : 
    int (*func_ptr)(int, int);
    func_ptr is a pointer to a function that takes two integers as arguments and returns an integer.

    example function:
    int add(int a, int b) {
        return a + b;
    }

    func_ptr = &add;

    what can we do with this pointer?
    how does it work?
 * 
 * 
 * 
 * 
 * 
*/

// ---------- Include Section ----------
#include <iostream>

// ---------- Using Section ----------
using std::cin;
using std::cout;
using std::endl;

const int SIZE = 4;

// ---------- Structs ----------

// ---------- Function Prototypes ----------

int add(int x, int y);
int subtract(int x, int y);
int multiply(int x, int y);
void performOperation(int x, int y, int (*operation)(int, int));
int divide(int x,int y);

// ---------- Main Function ----------
int main() {
   

    // if we had 3 different operations, than ALL get 2 ints and return an int
    // previosly we would do a nasty if else statement, and maybe with a enum, to decide which operation to do.
    // but now we can use a function pointer, and pass it as a parameter to a function.

    int a = 10;
    int b = 5;
    // its an array of function pointers, that get 2 ints and return an int.
    int (*operations[SIZE])(int, int) = {add, subtract, multiply, divide};

    for (int i = 0; i < SIZE; i++) {
        performOperation(a, b, operations[i]);
    }



    return 0;
}

// ---------- Functions ----------
int divide(int x,int y)
{
    return x / y;

}

// Function declarations
int add(int x, int y) {
    return x + y;
}

int subtract(int x, int y) {
    return x - y;
}

int multiply(int x, int y) {
    return x * y;
}

// Function pointer as a parameter
void performOperation(int x, int y, int (*operation)(int, int)) {
    int result = operation(x, y);
    cout << "result: " << result << endl;
}

