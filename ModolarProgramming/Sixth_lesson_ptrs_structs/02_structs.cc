/********************************************************************
 * Course: Modular Programming in C++   
 * Lecture: 06 - Ptrs and Structs
 * File: 02_structs.cc
 * 
 * Author: Yaniv Gabay
 * Date: 2024-05-13
 * 
 * Overview:
 * This file demonstrates the use of structs in C++ by defining a
 * simple struct `Point` and performing basic operations on it.
 * 
 * Notes:
 * - Structs are user-defined types that group variables.
 * - Direct assignment of structs is possible but not always recommended.
 *******************************************************************/

// ---------- Include Section ----------
#include <iostream>

// ---------- Using Section ----------
using std::cout;
using std::cin;
using std::endl;

// ---------- Constants ----------
const int MAX_SIZE = 100;
// Add more constants as needed

// ---------- Structs ----------
struct Point {
    int _x;
    int _y;
};

// Add more structs as needed

// ---------- Function Prototypes ----------
void printPoint(const Point& p); // Prototype for the print function

// ---------- Main Function ----------
int main() {
    Point p1 = {1, 2}; // Create a point this syntax works
    Point p2 = {3, 4}; // Create a point this syntax works

    printPoint(p1); // Print p1 details

    p1 = p2; // This does work, but it is not recommended
    cout << "after p1 = p2" << endl;
    printPoint(p1); // Print p1 details after assignment

    // We can create an empty point
    Point p3;
    // Expecting garbage values
    printPoint(p3); // Print p3 details
    // Assign values to p3
    p3._x = 5;
    p3._y = 6;
    printPoint(p3); // Print p3 details

    return 0;
}

// ---------- Functions ----------
// Function to print point details
//we send a const reference to the function to avoid copying the struct
//and avoid modifying the struct
void printPoint(const Point& p) {
    cout << "p: " << "x:" << p._x << ", " << "y:" << p._y << endl;
}

