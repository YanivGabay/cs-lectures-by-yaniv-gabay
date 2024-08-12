/********************************************************************
 * Course: Modular Programming in C++
 * Lecture: 06 - Pointers and Structs
 * File: .cc
 *
 * Author: Yaniv Gabay
 * Date: 2024-05-13
 *
 * Overview:
 * This example demonstrates the basic implementation of a custom String struct
 * in C++. The struct allows creating strings, printing them, and getting their length.
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
    size_t _length;

    // Default constructor
    String() {
        _length = 0;
        _str[0] = '\0';
    }

    // Constructor with parameters
    String(const char* str) {
        strcpy(_str, str);
        _length = strlen(str);
    }

    // Function to print the string
    void print() const {
        cout << _str << endl;
    }

    // Function to get the length of the string
    size_t getLength() const {
        return _length;
    }

    // Function to set the string value
    void setString(const char* str) {
        strcpy(_str, str);
        _length = strlen(str);
    }
};

// ---------- Function Prototypes ----------


// ---------- Main Function ----------
int main() {
    // Example using the default constructor
    String str;
    cout << "Default constructor - length: " << str.getLength() << endl; // 0
    str.setString("Hello");
    cout << "After setting string: ";
    str.print(); // Hello

    //could be int, but strlen returned size_t , so i prefer to use size_t
    size_t length = str.getLength();
    cout << "Length: " << length << endl; // 5

    // Example using the constructor with parameters
    String str2("World");
    cout << "Constructor with parameters: ";
    str2.print(); // World

    cout << "Length of str2: " << str2.getLength() << endl; // 5

    return 0;
}

// ---------- Functions ----------
// Add any additional functions if needed
