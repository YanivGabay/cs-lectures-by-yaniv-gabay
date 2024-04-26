/********************************************************************
 * Course: Modular Programming in C++
 * Lecture: 4 - Files
 * File: 3_flags.cc
 *
 * Author: Yaniv Gabay
 * Date: 2024-04-26
 *
 * Overview:


 * https://en.cppreference.com/w/cpp/io/basic_istream

 * Notes:

 *******************************************************************/

// ---------- Include Section ----------
#include <iostream>
// Add more #include directives as needed

#include <fstream>
// can include only some operation
#include <iomanip>
#include <cstring>


// ---------- Using Section ----------
using std::cin;
using std::cout;
using std::endl;

// ---------- Constants ----------
const int MAX_SIZE = 100;

// Add more constants as needed

// ---------- Function Prototypes ----------

// ---------- Main Function ----------
int main()
{
    //we can pass flags to the fsream file
    //for the full list of flags:
    // https://en.cppreference.com/w/cpp/io/basic_filebuf/open

    // std::ios::in - open for input operations
    // std::ios::out - open for output operations
   
    // std::ios::app - seek to the end of stream before each write
    std::fstream file;
    //this is fstream type
    // can be input or output, or both
    file.open("file1.txt", std::ios::in | std::ios::out);


    //fseek and ftell .
    //fseek - sets the file position indicator for the stream
    //ftell - returns the current value of the file position indicator for the stream
    return EXIT_SUCCESS;
}

// ---------- Functions ----------
