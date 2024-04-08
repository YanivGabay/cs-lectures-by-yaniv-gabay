/********************************************************************
 * Course: Modolar Programming in C++
 * Lecture: 2 - basic string
 * File: basic_strings.cc
 * 
 * Author: Yaniv Gabay
 * Date: 2024-04-08
 * 
 * Overview:
 * 
 * 
 * Notes:
 * 
 *******************************************************************/

// ---------- Include Section ----------
#include <iostream>
// Add more #include directives as needed
#include <iomanip>
// ---------- Using Section ----------
using std::cout;
using std::cin;
using std::endl;

// ---------- Constants ----------
const int MAX_SIZE = 5;
const int BIGGER = 50;

// ---------- Function Prototypes ----------

// ---------- Main Function ----------
int main() {
    // here nothing prevent us from enetering more chars than needed
    // depend on os, it can still print the whole thing, but it is sure not wanted behaviour
    char string[MAX_SIZE];
    cin >> string;
    cout << string << endl;
    // as you have been told, in order for our string to work with external functions like
    // std cmp ,copy    we need to use std::setw
    // so this will make sure, that we will not read more than MAX_SIZE
    // including the null terminator for strings 
    

    ///what will happend if we enter bigger?
    // the rest of the chars will stay in the buffer
    // and will be inserted into the code in the next section
    cin >> std::setw(MAX_SIZE)  >>string;
    cout << string << endl;
    cout << "now enterning input into an array size of 50" << endl;

    char sec_string[BIGGER];
    cin >> std::setw(BIGGER)  >>sec_string;
    cout <<sec_string << endl;
    // in this output, how come we dont print the rest of the array (figuring it has trash on it)
    // cus we took the input with setw so there is a null terminated after the last char

    //of course strings are arrays 
    // so we can do this
    cout << "going over the whole array (size 50) with a loop" << endl;
    for (int i = 0; i < BIGGER; i++) {
        cout << "the char at index " << i << " is: " <<"'" << sec_string[i] << "'" << " int value is:" << int(sec_string[i]) <<endl;
    }
  
    return 0;
}

// ---------- Functions ----------
