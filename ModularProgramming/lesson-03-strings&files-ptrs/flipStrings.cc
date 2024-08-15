/********************************************************************
 * Course: Modolar Programming in C++
 * Lecture: 3 - Strings
 * File: moreString.cc
 * 
 * Author: Yaniv Gabay
 * Date: 2024-04-14
 * 

 *******************************************************************/

// ---------- Include Section ----------
#include <iostream>

#include <cstring>
#include <iomanip>

// ---------- Using Section ----------
using std::cout;
using std::cin;
using std::endl;

// ---------- Constants ----------
const int MAX_SIZE = 100;


// ---------- Function Prototypes ----------

void smallestWord(const char[]);
// ---------- Main Function ----------
int main() {

    ///// given a string lets say:
    ///// "abc decv"
    // and given n for example n = 3 (can range from 2-4)
    // we need to "flip" the chars, every N time 
    // so to imagine it, we first "slice the string":
    //  "abc" " de" "cv"
    // we tried to "slice" it 3 chars each with whitespace chars included.
    // now we flip:
    // "cba" "ed " "cv" // the leftover are not needed to be flipped (less than N)


    //example:
    int n = 3;
    char example_string[] = "abc decv";
    cout << "string before mani: " << example_string << endl;
    for (int i = 0; i < strlen(example_string)/n; i++)
    {
        for (int j = i+n-1; example_string[j] != '\0' && j>i  ; j--)
        {
            //now we need to proc the "swap"
            char temp = example_string[i];
            example_string[i] = example_string[j];
            example_string[j] = temp;
            i++;
        }
        
    }
    cout << "string after mani: " << example_string << endl;

    return 0;
}
// ---------- Functions ----------
