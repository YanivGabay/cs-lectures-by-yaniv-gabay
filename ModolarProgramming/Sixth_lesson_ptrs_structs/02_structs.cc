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
const int MAX_SIZE = 100;
// Add more constants as needed

// ---------- Structs ----------
struct Point {
    int _x;
    int _y;
};

// Add more structs as needed

// ---------- Function Prototypes ----------


// ---------- Main Function ----------
int main() {
    Point p1 = {1, 2}; // create a point this syntax works
    Point p2 = {3, 4}; // create a point this syntax works

    cout << "p1: " << "x:"  <<p1._x << ", " << "y:" <<p1._y << endl;

    p1 = p2; // this does work, but it is not recommended
    cout << "after p1 = p2" << endl;
    cout << "p1: " << "x:"  <<p1._x << ", " << "y:" <<p1._y << endl;

    //we can create an empty point
    //not reccomended, also get warning from the compiler
    Point p3;
    // expecting garbage values
    cout << "p3: " << "x:"  <<p3._x << ", " << "y:" <<p3._y << endl; 
    //assign values to p3
    p3._x = 5;
    p3._y = 6;
    cout << "p3: " << "x:"  <<p3._x << ", " << "y:" <<p3._y << endl;

    
    return 0;
}

// ---------- Functions ----------
