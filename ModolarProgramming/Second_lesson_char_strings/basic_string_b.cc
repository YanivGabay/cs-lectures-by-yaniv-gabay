/********************************************************************
 * Course: Modolar Programming in C++
 * Lecture: 2 - basic string
 * File: basic_string_b.cc
 * 
 * Author: Yaniv Gabay
 * Date: 2024-04-08
 * 
 * Overview:
 * we will show some functions we can use, strcpy, strlen, strcat, strcmp
 * 
 * 
 * Notes:
 * 
 *******************************************************************/

// ---------- Include Section ----------
#include <iostream>
// Add more #include directives as needed
#include <iomanip>
#include <cstring>
// ---------- Using Section ----------
using std::cout;
using std::cin;
using std::endl;

// ---------- Constants ----------
const int MAX_SIZE = 5;
const int BIGGER = 50;

// ---------- Function Prototypes ----------
void printValue(int result);
// ---------- Main Function ----------
int main() {
    char string[MAX_SIZE];
    char sec_string[MAX_SIZE];

    cin >> std::setw(MAX_SIZE)  >>string;
    cout << string << endl;
    // to copy this string into the second array
    // we can use strcpy
    strcpy(sec_string, string); // target,source

    // go over the copy, if there is the letter a, we switch it to b
    // we KNOW that there is a '\0' at the end of the string
    // so this will be our term for the loop
    for (int i = 0; sec_string[i] != '\0'; i++) {
        if (sec_string[i] == 'a') {
            sec_string[i] = 'b';
        }
    }
    cout << "after changes: " << sec_string << endl;
    

    //str cmp
    // we can use strcmp to compare two strings
    //string are lexicographically compared
    // so abcd is smaller than abce (a,b,c,d) (=,=,=,<) (a,b,c,e)
    //str cmp returns:
    // 0 if the strings are equal
    // <0 if the first string is smaller lexically
    // >0 if the first string is bigger lexically
    std::cout << "comparing the two strings "<< "strcmp(string,sec_string): " << strcmp(string, sec_string) << std::endl;
    printValue(strcmp(string, sec_string));


    //strcat
    // we can use strcat to concatenate two strings
    //its our responsibility to make sure that the target string has enough space
    // to hold the result
    char third_string[MAX_SIZE + MAX_SIZE];
    strcpy(third_string, string);
    strcat(third_string, sec_string);
    cout << "after concatenation third_string has: " << third_string << endl;

    //strlen
    // we can use strlen to get the length of a string
    // it will return the number of characters in the string
    // not including the null terminator
    cout << "the length of the string is: " << strlen(string) << endl;
    cout << "the length of the sec_string is: " << strlen(sec_string) << endl;
    cout << "the length of the third_string is: " << strlen(third_string) << endl;
    return 0;


}

// ---------- Functions ----------
void printValue(int result)
{
    if(result == 0)
    {
        std::cout << "The strings are equal" << std::endl;
    }
    else if(result < 0)
    {
        std::cout << "The first string is smaller" << std::endl;
    }
    else
    {
        std::cout << "The first string is bigger" << std::endl;
    }
}
