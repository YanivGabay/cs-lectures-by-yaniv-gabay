/********************************************************************
 * Course: Modolar Programming in C++
 * Lecture: 1 - Chars Arrays
 * File: CharsArrays.cc
 * 
 * Author: Yaniv Gabay
 * Date: 2024-03-24
 * 
 * Overview:
 * Show some examples of using chars arrays in C++.
 * 
 * Notes:
 * 
 *******************************************************************/

// ---------- Include Section ----------
#include <iostream>
// Add more #include directives as needed

// ---------- Using Section ----------
using std::cout;
using std::cin;
using std::endl;

// ---------- Constants ----------
const int LENGTH = 5;
// Add more constants as needed

// ---------- Structs ----------


// ---------- Function Prototypes ----------
void exampleFunction(int parameter);
// Add more function prototypes as needed

// ---------- Main Function ----------
int main() {
    // Main function code
    char string[LENGTH] = {'H', 'e', 'l', 'l', 'o'};
    char input[LENGTH];
    
    int i = 0;
    do
    {
        cin >> input[i++];
    } while (!isspace(input[i-1]) && i < LENGTH);

    cout << input << endl;

    for (int  i = 0; i < LENGTH; i++)
    {
        cout << input[i];
    }
    cout << endl;
    //now we will do something different
    char input2[LENGTH];
    
    cin >> input2;

    for (int  i = 0; i < LENGTH; i++)
    {
        cout << input2[i];
    }
    cout << endl;
    cout << "cout<< char array:" <<input2 << endl;
    return 0;
}

// ---------- Functions ----------
void exampleFunction(int parameter) {
    // Example function implementation
    cout << "Parameter value: " << parameter << endl;
}
// Add more functions as needed