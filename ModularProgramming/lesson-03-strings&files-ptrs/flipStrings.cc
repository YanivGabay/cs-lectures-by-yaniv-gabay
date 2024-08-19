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
    int n = 4;
    char example_string[] = "Hello friend:)";
    cout << "string before mani: " << example_string << endl;
    cout << "string length: " << strlen(example_string) << endl;
    cout << "string length / n: " << strlen(example_string)/n << endl;
    
    for (int i = 0; i < strlen(example_string); i+=n)
    {
        int flip_index = i;
        for (int j = flip_index+n-1; j<strlen(example_string) && j>=flip_index  ; j--)
        {
            
            //now we need to proc the "swap"
            char temp = example_string[flip_index];
            example_string[flip_index] = example_string[j];
            example_string[j] = temp;
            flip_index++;
        }
        cout << "current i: " << i << " current string: " << example_string << endl;

       
        
    }
    cout << "string after mani: " << example_string << endl;

    return 0;
}
// ---------- Functions ----------
