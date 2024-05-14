/********************************************************************
 * Course: Modular Programming in C++   
 * Lecture: 06 - Ptrs and Structs
 * File: 05_evenMore.cc
 * 
 * Author: Yaniv Gabay
 * Date: 2024-05-13
 * 
 * Overview:
 * This file demonstrates the use of structs in C++ by defining 
 * simple structs `Point`, `Circle`, and `Rectangle` and performing 
 * basic operations on them.
 * 
 * Notes:
 * - Structs are user-defined types that group variables.
 * - Structs can be nested within other structs.
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
const int RADIUS = 5;

// ---------- Structs ----------
struct Point {
    int _x;
    int _y;
};
struct Circle {
    Point _center;
    int _radius;
};
struct Rectangle {
    Point _topLeft;
    Point _bottomRight;
};
// Add more structs as needed

// ---------- Function Prototypes ----------
void printCircle(const Circle& c);       // Prototype for printCircle function

// ---------- Main Function ----------
int main() {
    // Create an array of circles
    Circle circles[MAX_SIZE];

    // Initialize the array
    for (int i = 0; i < MAX_SIZE; i++) {
        circles[i]._center._x = i;
        circles[i]._center._y = i;
        circles[i]._radius = i + RADIUS;
    }

    // Print the array
    for (int i = 0; i < MAX_SIZE; i++) {
        printCircle(circles[i]);
    }

    // Reach the last element
    cout << "print the last element:" << endl;
    printCircle(circles[MAX_SIZE - 1]);

    // Create a pointer to a circle
    Circle* c = circles;
    // Print the first element
    cout << "print the first element using a pointer:" << endl;
    printCircle(*c);
    // Print the second element
    cout << "print the second element using a pointer:" << endl;
    printCircle(*(c + 1));

    return 0;
}

// ---------- Functions ----------
// Function to print circle details
void printCircle(const Circle& c) {
    cout << "Circle: center: x:" << c._center._x << ", y:" << c._center._y << ", radius:" << c._radius << endl;
}
