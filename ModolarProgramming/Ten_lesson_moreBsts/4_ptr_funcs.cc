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

// ---------- Structs ----------

// ---------- Function Prototypes ----------

int add(int x, int y);
int subtract(int x, int y);
int multiply(int x, int y);
void performOperation(int x, int y, int (*operation)(int, int));


// ---------- Main Function ----------
int main() {
   

    // if we had 3 different operations, than ALL get 2 ints and return an int
    // previosly we would do a nasty if else statement, and maybe with a enum, to decide which operation to do.
    // but now we can use a function pointer, and pass it as a parameter to a function.

    int a = 10;
    int b = 5;

    int (*operation)(int, int); // function ptr;

    operation = &add; // opeartion = add also works, the & just so we know its pointers.
    
    performOperation(a, b, operation);
    performOperation(a, b, &subtract); //we are sending the function name, and not the substract() call.
    performOperation(a, b, &multiply);
    



    return 0;
}

// ---------- Functions ----------


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

