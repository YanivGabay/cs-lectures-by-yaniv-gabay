/********************************************************************
 * Course: Modolar Programming in C++
 * Lecture: 3 - Strings
 * File: moreString.cc
 * 
 * Author: Yaniv Gabay
 * Date: 2024-04-14
 * 
 * Overview:
 * This program will be a string analayzer, it will take a string from the user
 * and will print the following:
 * 1. the length of the string
 * 2. the number of words in the string
 * 3. the number of letters in the string
 * 4. the number of digits in the string
 * 5. the number of special characters in the string
 * 6. the number of spaces in the string
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
void stringChecker(const char[]);
void lengthChecker(const char[]);
void wordCounter(const char[]);
void letterCounter(const char[]);
void digitCounter(const char[]);
void specialCharCounter(const char[]);
void spaceCounter(const char[]);
void biggestWord(const char[]);
void smallestWord(const char[]);
// ---------- Main Function ----------
int main() {
    char user_input[MAX_SIZE] = {0};
    
    cout << "Please enter a string: ";
    cin.getline(user_input, MAX_SIZE);


    //this wont work, i want a string with spaces:
    // cin >> setw(MAX_SIZE) >> user_input;


    stringChecker(user_input);

    return 0;
}
// ---------- Functions ----------
// =================================================================
void stringChecker(const char input[])
{
    cout << "The string is: " << input << endl;
    cout << "== String Analysis ==" << endl;
    lengthChecker(input);
    wordCounter(input);
    letterCounter(input);
    digitCounter(input);
    specialCharCounter(input);
    spaceCounter(input);
    biggestWord(input);
    smallestWord(input);


    cout << "====================" << endl;
    cout << "End of Analysis" << endl;
    cout << "====================" << endl;
    cout << "Thank you for using our program" << endl;

}
// =================================================================
void lengthChecker(const char input[])
{
    cout << "The length of the string is: " << strlen(input) << endl;
}
// =================================================================
void wordCounter(const char input[])
{
    int words = 0;
    bool inWord = false;
    for(int i = 0; i < strlen(input); i++)
    {
        if(input[i] == ' ')
        {
            inWord = false;
        }
        else if(!inWord)
        {
            inWord = true;
            words++;
        }
    }
    cout << "The number of words in the string is: " << words << endl;
}
// =================================================================
// you will learn, after learning pointers, and functions pointers
// how to write all those counters in a single function, where each time
// we will send another function as an arguemtn
void letterCounter(const char input[])
{
    int letters = 0;
    for(int i = 0; i < strlen(input); i++)
    {
        if(isalpha(input[i]))
        {
            letters++;
        }
    }
    cout << "The number of letters in the string is: " << letters << endl;
}
// =================================================================
void digitCounter(const char input[])
{
    int digits = 0;
    for(int i = 0; i < strlen(input); i++)
    {
        if(isdigit(input[i]))
        {
            digits++;
        }
    }
    cout << "The number of digits in the string is: " << digits << endl;
}
// =================================================================
void specialCharCounter(const char input[])
{
    int specialChars = 0;
    for(int i = 0; i < strlen(input); i++)
    {
        //is alnum checks if the character is a letter or a digit
        //so if it isnt, and it isnt a space, it is a special character
        if(!isalnum(input[i]) && !isspace(input[i]))
        {
            specialChars++;
        }
    }
    cout << "The number of special characters in the string is: " << specialChars << endl;
}
// =================================================================
void spaceCounter(const char input[])
{
    int spaces = 0;
    for(int i = 0; i < strlen(input); i++)
    {
        if(isspace(input[i]))
        {
            spaces++;
        }
    }
    cout << "The number of spaces in the string is: " << spaces << endl;
}
// =================================================================
void biggestWord(const char input[])
{
    int biggestWord = 0;
    int currentWord = 0;

    
    char biggestWordString[MAX_SIZE] = {0};

    for(int i = 0; i < strlen(input); i++)
    {
        if(isalnum(input[i]))
        {
            currentWord++;
        }
        else
        {
            if(currentWord > biggestWord)
            {
                biggestWord = currentWord;
                strncpy(biggestWordString, input + i - currentWord, currentWord);
            }
            currentWord = 0;
        }
    }
    cout << "The biggest word in the string is: " << biggestWordString << endl;
    cout << "The length of the biggest word in the string is: " << biggestWord << endl;
}
// =================================================================
void smallestWord(const char input[])
{
    int smallestWord = 0;
    int currentWord = 0;

    char smallestWordString[MAX_SIZE] = {0};

    for(int i = 0; i < strlen(input); i++)
    {
        if(isalnum(input[i]))
        {
            currentWord++;
        }
        else
        {
            if(currentWord < smallestWord || smallestWord == 0)
            {
                smallestWord = currentWord;
                strncpy(smallestWordString, input + i - currentWord, currentWord);
                smallestWordString[currentWord] = '\0';
            }
            currentWord = 0;
        }
    }
    cout << "The smallest word in the string is: " << smallestWordString << endl;
    cout << "The length of the smallest word in the string is: " << smallestWord << endl;
}
