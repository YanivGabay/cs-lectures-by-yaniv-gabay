/********************************************************************
 * Course: Modular Programming in C++
 * Lecture: 06 - Pointers and Structs
 * File: .cc
 *
 * Author: Yaniv Gabay
 * Date: 2024-05-13
 *
 * Overview:
 * This example demonstrates the basic implementation of a singly linked list
 * in C++. The linked list allows adding elements and printing the list.
 *******************************************************************/

// ---------- Include Section ----------
#include <iostream>
#include <cstring>
// ---------- Using Section ----------
using std::cin;
using std::cout;
using std::endl;


const int MAX = 50;

// ---------- Structs ----------
struct String {
    char _str[MAX];
    int _length;

    void print() {
        cout << _str << endl;
    }
    int getLength() {
        return _length;
    }

};


// ---------- Function Prototypes ----------


// ---------- Main Function ----------
int main() {
    
    String str;
    strcpy(str._str, "Hello");
    str._length = 5;
    str.print();


    int length = str.getLength();
    cout << "Length: " << length << endl;
    return 0;
}

// ---------- Functions ----------

