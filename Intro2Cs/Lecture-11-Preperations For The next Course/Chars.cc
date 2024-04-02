/********************************************************************
 * Course: Modolar Programming in C++
 * Lecture: 1 - Chars
 * File: Chars.cc
 * 
 * Author: Yaniv Gabay 
 * Date: 2024-03-24
 * 
 * Overview:
 * Show some examples of using chars in C++.
 * 
 * Notes:
 * We will understand ascii and how to use chars in C++.

 *******************************************************************/

// ---------- Include Section ----------
#include <iostream>
// Add more #include directives as needed
#include <cctype>  // new include !
// ---------- Using Namespace ----------
using std::cout;
using std::cin;
using std::endl;


// ---------- Constants ----------
const int MAX_SIZE = 100;
// Add more constants as needed

// ---------- Structs ----------

// ---------- Function Prototypes ----------
void exampleFunction(int parameter);
// Add more function prototypes as needed

// ---------- Main Function ----------
int main() {
    // there are 256 ascii characters
    // we can google ASCI TABLE and see all the characters
    for (int i = 0; i < 256; i++)
    {
        // print i , and than we cast i to char!
        // so give us the char, that the number represent
        // for example 65 is A
        cout << i << "=" << char(i) << endl;
    }
    
    cout << "Enter a character: ";
    char input; // char variable
    cin >> input; // get the input from the user
    
    if(input >= 'a' && input <= 'z') {
        cout << "Lower case letter" << endl;
    } else if(input >= 'A' && input <= 'Z') {
        cout << "Upper case letter" << endl;
    } else if(input >= '0' && input <= '9') {
        cout << "Digit" << endl;
    } else {
        cout << "Special character" << endl;
    }
    
    // very important to know this ascii values:
    // 0 = \0
    // 10 = \n  
    // there is also \t for tab
    // \n \t etc are called escape characters or 
    // white space characters
    //we will return to them later! 
    
    // we can use the cctype library to check if the char is digit or letter
    if(isdigit(input)) {
        cout << "Digit" << endl;
    } else if(isalpha(input)) {
        cout << "Letter" << endl;
    } else {
        cout << "Special character" << endl;
    }

    // this part we wont focus, but because chars have a number representation
    // we can manipulate them as numbers
    // for example we can add 1 to a char, and it will move to the next char
    // for example 'a' + 1 = 'b'
    // we can also subtract chars, and get the distance between them
    // for example 'b' - 'a' = 1


    return 0;
}

