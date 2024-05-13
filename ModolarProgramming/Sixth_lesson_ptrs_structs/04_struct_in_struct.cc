/********************************************************************
 * Course: Modolar Programming in C++   
 * Lecture: 06 - Ptrs and Structs
 * File: 02_structs.cc
 * 
 * Author: Yaniv Gabay
 * Date: 2024-05-13
 * 
 * Overview:

 * 
 * Notes:

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


// ---------- Main Function ----------
int main() {
    //create a circle
    Circle c = {{1, 2}, 3}; // this works.
    cout << "printing c:" << endl;
    cout << "c: " << "center: " << "x:" << c._center._x << ", y:" << c._center._y << ", radius:" << c._radius << endl;

    //we can do it first creating a point
    Point p = {4, 5};
    Circle c2 = {p, 6}; // this works.
    cout << "printing c2:" << endl;
    cout << "c2: " << "center: " << "x:" << c2._center._x << ", y:" << c2._center._y << ", radius:" << c2._radius << endl;
    
    //create a rectangle
    Rectangle r = {{1, 2}, {3, 4}}; // this works.
    cout << "printing r:" << endl;
    cout << "r: " << "topLeft: " << "x:" << r._topLeft._x << ", y:" << r._topLeft._y << ", bottomRight: " << "x:" << r._bottomRight._x << ", y:" << r._bottomRight._y << endl;

    //we can do it first creating two points
    Point topLeft = {5, 6};
    Point buttomRight = {7, 8};
    Rectangle r2 = {topLeft, buttomRight}; // this works.
    cout << "printing r2:" << endl;
    cout << "r2: " << "topLeft: " << "x:" << r2._topLeft._x << ", y:" << r2._topLeft._y << ", bottomRight: " << "x:" << r2._bottomRight._x << ", y:" << r2._bottomRight._y << endl;


    return 0;
}

// ---------- Functions ----------
