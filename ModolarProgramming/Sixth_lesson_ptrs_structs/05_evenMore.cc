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


// ---------- Main Function ----------
int main() {
    //create an array of circles
    Circle circles[MAX_SIZE];

    //initialize the array

    for (int i = 0; i < MAX_SIZE; i++) {
        circles[i]._center._x = i;
        circles[i]._center._y = i;
        circles[i]._radius = i+RADIUS;
    }

    //print the array
    for (int i = 0; i < MAX_SIZE; i++) {
        cout << "circles[" << i << "]: " << "center: " << "x:"  <<circles[i]._center._x << ", " << "y:" <<circles[i]._center._y << ", radius:" << circles[i]._radius << endl;
    }

    //reach the last element
    cout << "print the last element:" << endl;
    cout << "circles[" << MAX_SIZE-1 << "]: " << "center: " << "x:"  <<circles[MAX_SIZE-1]._center._x << ", " << "y:" <<circles[MAX_SIZE-1]._center._y << ", radius:" << circles[MAX_SIZE-1]._radius << endl;

    //create a pointer to a circle
    Circle* c = circles;
    //print the first element
    cout << "print the first element using a pointer:" << endl;
    cout << "c: " << "center: " << "x:"  <<c->_center._x << ", " << "y:" <<c->_center._y << ", radius:" << c->_radius << endl;
    //we could have also used c._center._x, c._center._y, c._radius
    //print the second element
     cout << "print the second element using a pointer:" << endl;
    cout << "c+1: " << "center: " << "x:"  <<(c+1)->_center._x << ", " << "y:" <<(c+1)->_center._y << ", radius:" << (c+1)->_radius << endl;

   


    return 0;
}

// ---------- Functions ----------
