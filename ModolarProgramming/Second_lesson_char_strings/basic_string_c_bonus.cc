/********************************************************************
 * Course: Modolar Programming in C++
 * Lecture: 2 - basic string
 * File: basic_string_c_bonus.cc
 * 
 * Author: Yaniv Gabay
 * Date: 2024-04-08
 * 
 * Overview:
 * previosly we have shown: strcpy, strlen, strcat, strcmp
 * there are many more functions in the cstring library
 * this is some of my favorites 
 * link:  https://en.cppreference.com/w/cpp/header/cstring
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
void checkPrefix(const char string[], const char prefix_string[],int length);
// ---------- Main Function ----------
int main() {
    char string[MAX_SIZE];
    char sec_string[MAX_SIZE];
    //atoi is very popular and useful function
    // it converts a string to an integer
    // if the string is not a number, it will return 0
    // if the string is a number, it will return the number
    // if the string is a number with letters, it will stop
    cout << atoi("123") << endl;
    cout << atoi("123a") << endl;
    cout << atoi("a123") << endl;
    cout << atoi("a") << endl;
    cout << atoi("123a123") << endl;

    //strncmp
    // this function is similar to strcmp
    // but it takes a third argument, the number of chars to compare
    // so strncmp("abc","abcd",3) will return 0
    // strncmp("abc","abcd",4) will return -1
    cout << "====================" << endl;
    cout << "strncmp" << endl;
    
    checkPrefix("Hello, world!", "Hello, everybody!", 13);
    checkPrefix("Hello, everybody!", "Hello, world!", 13);
    checkPrefix("Hello, everybody!", "Hello, world!", 7);
    checkPrefix("Hello, everybody!" + 12, "Hello, somebody!" + 11, 5);

    cout << "====================" << endl;
    cout << "strncat" << endl;
    //strncat
    // this function is similar to strcat
    // but it takes a third argument, the number of chars to copy
    char string_to_grow[BIGGER] = "Hello";
    char string2_to_cat[BIGGER] = "world!";
    int length = 3;
    cout << "we will add: " << length << " chars from '" << string2_to_cat << "' to '" << string_to_grow << "'"<< endl;
    strncat(string_to_grow, string2_to_cat, length);
    
    cout << "string_to_grow after strnncat: "  <<string_to_grow << endl;


    //there are other functions, but they need knowledge of pointers
    // which you still dont know, but look up strstr , strchr, strrchr

    //example of strrchr
    
    cout << "====================" << endl;
    cout << "strrchr" << endl;
    char string1[] = "Hello, world!";
    char* string_after_last_occurence = strrchr(string1, 'o');
    if (string_after_last_occurence)
    {
        cout << "The string after the occurence of 'o' is: " << string_after_last_occurence << endl;
    }
    else
    {
        cout << "The char 'o' was not found" << endl;
    }
  
    




    return 0;


}

// ---------- Functions ----------

void checkPrefix(const char string[], const char prefix_string[],int length)
{
    if(strncmp(string,prefix_string,length) == 0)
    {
        cout << "The string starts with the prefix" << endl;
    }
    else if (strncmp(string,prefix_string,length) < 0)
    {
        cout << "The string: " << string <<" is smaller than the prefix " << prefix_string << endl;
    }
    else
    {
        cout << "The string: " << string <<" is bigger than the prefix " << prefix_string << endl;
        
    }
    //can just != 0 meaning the string doesnt start with the prefix
}



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
