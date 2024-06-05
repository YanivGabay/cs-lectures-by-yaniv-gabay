/********************************************************************
 * Course: Modolar Programming
 * Lecture: 8 - Exercise 2 Linked lists _ some trees
 * File: cin_eof.cc
 * 
 * Author: Yaniv Gabay
 * Date: 2024-06-CURRENT_DAY
 * 
 * Overview:
 * we will go over some usefull things about the second exercise in the eighth lesson.
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
const int MAX_SIZE = 100;
// Add more constants as needed

// ---------- Structs ----------


// ---------- Function Prototypes ----------


// ---------- Main Function ----------
int main() {
    



    while (true)  
    {
        int value;
        cin >> value;
        if(cin.eof()==true)
        {
            cout << "cin.eof status: " << cin.eof() << endl;
            break;
        }
        cout << "Value: " << value << endl;
       
    }
    cin.ignore(); // Clear the cin buffer

    cout << "cin.eof status: " << cin.eof() << endl;
    return 0;
}

// ---------- Functions ----------
