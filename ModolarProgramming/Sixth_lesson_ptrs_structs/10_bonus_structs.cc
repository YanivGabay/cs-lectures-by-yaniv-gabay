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
    String() { // constructor // you will learn about constructors in the next year
        _length = 5;
        _str[0] = '\0';
    }
    //can create a constructor with parameters
    String(const char* str) {
        strcpy(_str, str);
        _length = strlen(str);
    }
};


// ---------- Function Prototypes ----------


// ---------- Main Function ----------
int main() {
    
    String str;
    cout << "length: " << str._length << endl; // 0
    strcpy(str._str, "Hello");
    str._length = 5;
    str.print();


    int length = str.getLength();
    cout << "Length: " << length << endl;

    //example of a constructor with parameters
    String str2("World");
    str2.print();
    return 0;
}

// ---------- Functions ----------

