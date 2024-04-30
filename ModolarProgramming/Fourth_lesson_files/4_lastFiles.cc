/********************************************************************
 * Course: Modular Programming in C++
 * Lecture: 4 - Files
 * File: 4_lastFiles.cc
 *
 * Author: Yaniv Gabay
 * Date: 2024-04-26
 *
 * Overview:

 * just a small example, we can open multiple files
 * and close them in a loop
 * just to remember we can array everything we want
 * and use loops to iterate over them
 * this is also is a nice example, cus if we failed reading one of the files
 * we will close all the files that we opened before
 * and return false
 * 
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
const char FILE_NAME[] = "third_example.txt";
// Add more constants as needed

const int NUM_FILES = 5;

//two dimensional array of files
const char* filenames[NUM_FILES] = {"read1.txt", "read2.txt", "read3.txt", "read4", "read5.txt"};
// ---------- Function Prototypes ----------
bool checkOpen(std::fstream& file,const int index);
bool openFiles(std::fstream array[]);
void closeFiles(std::fstream array[],const int index);
// ---------- Main Function ----------
int main()
{

    // of course we can create an array of fstream
    // and open all of them in a loop


    std::fstream array[NUM_FILES];
    if(!openFiles(array))
    {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

// ---------- Functions ----------
bool openFiles(std::fstream array[])
{
   for (int i = 0; i < NUM_FILES; ++i) {

        array[i].open(filenames[i], std::ios::in); // Open file for reading

        if(!checkOpen(array[i],i))
        {
            closeFiles(array,i);
            return false;
        }
        else
        {
            std::cout << "File " << filenames[i] << " opened successfully." << std::endl;
            // Process file here if needed
        }
      
    }
    return true;
}
bool checkOpen(std::fstream& file,const int index)
{
    if (!file.is_open())
    {
        std::cerr << "Error opening file" << filenames[index] << std::endl;
        return false;
    }
    return true;
}

void closeFiles(std::fstream array[],const int index)
{
    for (int i = 0; i < index; ++i) {
        std::cout << "Closing file " << filenames[i] << std::endl;
        array[i].close();
    }
}