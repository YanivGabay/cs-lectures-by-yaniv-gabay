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
void deleteCharFromString(char str[], char c);
// ---------- Main Function ----------
int main() {
    int value = 5;
    char c = (char)(value+'0');
    cout << c << endl;

    char array[] = "Hell";
    char array2[15] = "Hello";
    //strcat(array,c); //this dont work? what we should do?
    //str cat only accept STRINGs, which are chars arrays with \0 at the end
    //we have to convert the char to a string
    char str[2] = {c, '\0'}; // works
    //than
    

    strcat(array2,str);// will work.
    cout << "string before the deletechar function:" << array << endl;
    deleteCharFromString(array,'l');

    return 0;
}
// ---------- Functions ----------
// =================================================================
//unique way to delete a char from a string

// [hel] [l]
// str[replace] = h
// str[to_delete] = h
// str[to_delete] = str[replace] = h
// to_delete = 1
// replace = 1
// str[replace] = e
// str[to_delete] = e
// str[to_delete] = str[replace] = e
// to_delete = 2
// replace = 2
// str[replace] = l
// str[replace] == char_to_delete
// replace = 3
// str[replace] == \0 //exit loop
// str[to_delete] = \0 // to_delete = 2;
// str[0] = h
// str[1] = e
// str[2] = \0

void deleteCharFromString(char str[], char char_to_delete){
    int replace = 0;
    int to_delete = 0;
    while(str[replace] != '\0'){
        if(str[replace] != char_to_delete){
            str[to_delete] = str[replace];
            to_delete++;
        }
        replace++;
    }
    str[to_delete] = '\0';
    cout << "result of the delte function:" << str << endl;
}