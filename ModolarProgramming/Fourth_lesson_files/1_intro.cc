/********************************************************************
 * Course: Modular Programming in C++
 * Lecture: 4 - Files
 * File: 1_intro.cc
 * 
 * Author: Yaniv Gabay
 * Date: 2024-04-26
 * 
 * Overview:
 * we show some basic cin operations,like cin.eof(),
 * it will work with files as well.
 * after that some basic files operations.
 * https://en.cppreference.com/w/cpp/io/basic_istream
 * to see the operations available for cin.
 * Notes:

 *******************************************************************/

// ---------- Include Section ----------
#include <iostream>
// Add more #include directives as needed
#include <fstream>
// ---------- Using Section ----------
using std::cout;
using std::cin;
using std::endl;

// ---------- Constants ----------
const int MAX_SIZE = 100;
// Add more constants as needed



// ---------- Function Prototypes ----------
void basicInput();
void alternateInput();
void cleanBuffer();
// ---------- Main Function ----------
int main() {
    

    basicInput();
    cout << "-------------------\n";


    //we need to clear the cin flags
    // cus the cin.fail() is still "on"
    // so we need to clear it.
    cin.clear();
    cleanBuffer();
    //to eat the '\n' from the buffer
    alternateInput();
    return 0;
}

// ---------- Functions ----------
void alternateInput()
{
     //cin will fail, when we try to 
    // read a char instead of int
    
    int num = 0;
    int counter = 0;
    cin >> num;
    while (cin.good()) // cin.good == true
    {
        counter++;
        cout << "Number " << counter << ": " << num << endl;
        cin >> num;
    }
    cout << "cin.good has return false\n";
    cout << "Total numbers: " << counter << endl;
}
void basicInput()
{
    //cin will fail, when we try to 
    // read a char instead of int
    int num = 0;
    int counter = 0;
    cin >> num;
    while (cin.fail() == false) // !cin.eof()
    {
        counter++;
        cout << "Number " << counter << ": " << num << endl;
        cin >> num;
    }
    cout << "cin.fail has return true\n";
    cout << "Total numbers: " << counter << endl;

    
}
void cleanBuffer()
{
    //to eat the '\n' from the buffer
    char temp = cin.get();
    while(temp != '\n' && temp != EOF)
    {
        temp = cin.get();
    }
}
