/********************************************************************
 * Course: Modular Programming in C++   
 * Lecture: 06 - Ptrs and Structs
 * File: 04_struct_in_struct.cc
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
void printPoint(const Point& p);         // Prototype for printPoint function
void printCircle(const Circle& c);       // Prototype for printCircle function
void printRectangle(const Rectangle& r); // Prototype for printRectangle function

// ---------- Main Function ----------
int main() {
    // Create a circle
    Circle c = {{1, 2}, 3}; // this works.
    cout << "printing c:" << endl;
    printCircle(c);

    // We can do it first by creating a point
    Point p = {4, 5};
    Circle c2 = {p, 6}; // this works.
    cout << "printing c2:" << endl;
    printCircle(c2);
    
    // Create a rectangle
    Rectangle r = {{1, 2}, {3, 4}}; // this works.
    cout << "printing r:" << endl;
    printRectangle(r);

    // We can do it first by creating two points
    Point topLeft = {5, 6};
    Point bottomRight = {7, 8};
    Rectangle r2 = {topLeft, bottomRight}; // this works.
    cout << "printing r2:" << endl;
    printRectangle(r2);

    return 0;
}

// ---------- Functions ----------
// Function to print point details
void printPoint(const Point& p) {
    cout << "Point: " << "x:" << p._x << ", y:" << p._y << endl;
}

// Function to print circle details
void printCircle(const Circle& c) {
    cout << "Circle: center: x:" << c._center._x << ", y:" << c._center._y << ", radius:" << c._radius << endl;
}

// Function to print rectangle details
void printRectangle(const Rectangle& r) {
    cout << "Rectangle: topLeft: x:" << r._topLeft._x << ", y:" << r._topLeft._y << ", bottomRight: x:" << r._bottomRight._x << ", y:" << r._bottomRight._y << endl;
}
