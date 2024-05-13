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
Circle * createCircles();
Circle * betterCreateCircles();
// ---------- Main Function ----------
int main() {
    //we will create an array of circles but through a function


    //BAD EXAMPLE, WHY THIS IS BAD?????????
    Circle * ptr = createCircles();
      //print the array
    //for (int i = 0; i < MAX_SIZE; i++) {
   //     cout << "circles[" << i << "]: " << "center: " << "x:" << ptr[i]._center._x << ", y:" << ptr[i]._center._y << ", radius:" << ptr[i]._radius << endl;
   // }

    //so if we want to fix
    //we need to dynamically allocate the memory
    ptr = betterCreateCircles();
      for (int i = 0; i < MAX_SIZE; i++) {
        cout << "circles[" << i << "]: " << "center: " << "x:" << ptr[i]._center._x << ", y:" << ptr[i]._center._y << ", radius:" << ptr[i]._radius << endl;
    }

    //free the memory
    delete[] ptr;
    ptr = nullptr;

    return 0;
}

// ---------- Functions ----------
Circle * createCircles() {
    //create an array of circles
    Circle circles[MAX_SIZE];

    //initialize the array

    for (int i = 0; i < MAX_SIZE; i++) {
        circles[i]._center._x = i;
        circles[i]._center._y = i;
        circles[i]._radius = i+RADIUS;
    }

  

    return circles;
}

Circle * betterCreateCircles() {
    //create an array of circles
    Circle * circles = new Circle[MAX_SIZE];

    //initialize the array

    for (int i = 0; i < MAX_SIZE; i++) {
        circles[i]._center._x = i;
        circles[i]._center._y = i;
        circles[i]._radius = i+RADIUS;
    }

  

    return circles;
}