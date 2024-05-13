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

// Add more structs as needed

// ---------- Function Prototypes ----------


// ---------- Main Function ----------
int main() {
    //we can create an array of points
    Point points[MAX_SIZE];
    //initialize the array
    for (int i = 0; i < MAX_SIZE; i++) {
        points[i]._x = i;
        points[i]._y = i;
    }

    //print the array
    for (int i = 0; i < MAX_SIZE; i++) {
        cout << "points[" << i << "]: " << "x:"  <<points[i]._x << ", " << "y:" <<points[i]._y << endl;
    }

    //reach the last element
    cout << "print the last element:" << endl;
    cout << "points[" << MAX_SIZE-1 << "]: " << "x:"  <<points[MAX_SIZE-1]._x << ", " << "y:" <<points[MAX_SIZE-1]._y << endl;

    //create a pointer to a point
    Point* p = points;
    //print the first element
    cout << "print the first element using a pointer:" << endl;
    cout << "p: " << "x:"  <<p->_x << ", " << "y:" <<p->_y << endl;
    //print the second element
     cout << "print the second element using a pointer:" << endl;
    cout << "p+1: " << "x:"  <<(p+1)->_x << ", " << "y:" <<(p+1)->_y << endl;
    //print the third element
     cout << "print the third element using a pointer:" << endl;
    cout << "p+2: " << "x:"  <<(p+2)->_x << ", " << "y:" <<(p+2)->_y << endl;
    //print the last element
     cout << "print the last element using a pointer:" << endl;
    cout << "p+MAX_SIZE-1: " << "x:"  <<(p+MAX_SIZE-1)->_x << ", " << "y:" <<(p+MAX_SIZE-1)->_y << endl;

    
    return 0;
}

// ---------- Functions ----------
