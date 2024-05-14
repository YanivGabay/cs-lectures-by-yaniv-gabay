/********************************************************************
 * Course: Modular Programming in C++   
 * Lecture: 06 - Ptrs and Structs
 * File: 02_structs.cc
 * 
 * Author: Yaniv Gabay
 * Date: 2024-05-13
 * 
 * Overview:
 * This file demonstrates the use of structs and pointers in C++ by defining a
 * simple struct `Point`, initializing an array of points, and performing 
 * various operations on them.
 * 
 * Notes:
 * - Structs are user-defined types that group variables.
 * - Arrays of structs can be used to store multiple instances.
 * - Pointers can be used to access array elements.
 *******************************************************************/

// ---------- Include Section ----------
#include <iostream>

// ---------- Using Section ----------
using std::cout;
using std::cin;
using std::endl;

// ---------- Constants ----------
const int MAX_SIZE = 5;
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
    // We can create an array of points
    Point points[MAX_SIZE];
    // Initialize the array
    for (int i = 0; i < MAX_SIZE; i++) {
        points[i]._x = i;
        points[i]._y = i;
    }

    // Print the array
    for (int i = 0; i < MAX_SIZE; i++) {
        printPoint(points[i]);
    }

    // Reach the last element
    cout << "print the last element:" << endl;
    printPoint(points[MAX_SIZE - 1]);

    // Create a pointer to a point
    Point* p = points;



    //what this will print???
    //cout << p->_x << endl; 

    // Print the first element
    cout << "print the first element using a pointer:" << endl;
    //content of p is the address of the first element
    //which is a point struct
    printPoint(*p);
    // Print the second element
    cout << "print the second element using a pointer:" << endl;
    printPoint(*(p + 1));
    // Print the third element
    cout << "print the third element using a pointer:" << endl;
    printPoint(*(p + 2));
    // Print the last element
    cout << "print the last element using a pointer:" << endl;
    printPoint(*(p + MAX_SIZE - 1));

    return 0;
}

// ---------- Functions ----------
// Function to print point details
void printPoint(const Point& p) {
    cout << "Point: " << "x:" << p._x << ", " << "y:" << p._y << endl;
}
