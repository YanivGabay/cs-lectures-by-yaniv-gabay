/********************************************************************
 * Course: Modolar Programming in C++
 * Lecture: 3 - Strings
 * File: returnToChars.cc
 * 
 * Author: Yaniv Gabay
 * Date: 2024-04-14
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
#include <cstring>
#include <iomanip>

// ---------- Using Section ----------
using std::cout;
using std::cin;
using std::endl;

// ---------- Constants ----------
const int MAX_SIZE = 100;


// ---------- Function Prototypes ----------

// ---------- Main Function ----------
int main() {
    int value = 5;
    char c = (char)(value+'0');
    cout << c << endl;

    char array[] = "Hello";

    //strcat(array,c); //this dont work? what we should do?
    //str cat only accept STRINGs, which are chars arrays with \0 at the end

    
    return 0;
}
// ---------- Functions ----------
// =================================================================
