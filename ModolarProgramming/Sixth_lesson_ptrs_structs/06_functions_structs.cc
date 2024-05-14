/********************************************************************
 * Course: Modular Programming in C++   
 * Lecture: 06 - Ptrs and Structs
 * File: 06_functions_structs.cc
 * 
 * Author: Yaniv Gabay
 * Date: 2024-05-13
 * 
 * Overview:
 * This file demonstrates the use of structs and dynamic memory allocation
 * in C++ by defining simple structs `Point`, `Circle`, and `Rectangle`, 
 * and performing basic operations on them.
 * 
 * Notes:
 * - Structs are user-defined types that group variables.
 * - Dynamic memory allocation is used to create an array of structs.
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
Circle* createCircles();
Circle* betterCreateCircles();
void printCircle(const Circle& c); // Prototype for printCircle function

// ---------- Main Function ----------
int main() {
    // We will create an array of circles but through a function

    // BAD EXAMPLE, WHY THIS IS BAD???????
    Circle* ptr = createCircles();
    // print the array (commented out because it's a bad example)
    // for (int i = 0; i < MAX_SIZE; i++) {
    //     cout << "circles[" << i << "]: " << "center: " << "x:" << ptr[i]._center._x << ", y:" << ptr[i]._center._y << ", radius:" << ptr[i]._radius << endl;
    // }

    // So if we want to fix
    // We need to dynamically allocate the memory
    ptr = betterCreateCircles();
    for (int i = 0; i < MAX_SIZE; i++) {
        printCircle(ptr[i]);
    }

    // Free the memory
    delete[] ptr;
    ptr = nullptr;

    return 0;
}

// ---------- Functions ----------
Circle* createCircles() {
    // Create an array of circles
    Circle circles[MAX_SIZE];

    // Initialize the array
    for (int i = 0; i < MAX_SIZE; i++) {
        circles[i]._center._x = i;
        circles[i]._center._y = i;
        circles[i]._radius = i + RADIUS;
    }

    // Return a pointer to the local array (bad practice)
    return circles;
}

Circle* betterCreateCircles() {
    // Create an array of circles
    Circle* circles = new Circle[MAX_SIZE];

    // Initialize the array
    for (int i = 0; i < MAX_SIZE; i++) {
        circles[i]._center._x = i;
        circles[i]._center._y = i;
        circles[i]._radius = i + RADIUS;
    }

    return circles;
}

// Function to print circle details
void printCircle(const Circle& c) {
    cout << "Circle: center: x:" << c._center._x << ", y:" << c._center._y << ", radius:" << c._radius << endl;
}
