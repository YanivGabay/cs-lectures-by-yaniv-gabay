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
const char FILE_NAME[] = "third_example.txt";
// Add more constants as needed

// ---------- Function Prototypes ----------
void expirement_tell(std::fstream& file);
void more_seek(std::fstream& file);
void expirement_seek(std::fstream& file);
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
    file.open(FILE_NAME, std::ios::in | std::ios::out);
    if(!file)
    {
        std::cerr << "Error opening file\n";
        return EXIT_FAILURE;
    }
    //this will open the file for input and output


  




    //fseek and ftell .
    //fseek - sets the file position indicator for the stream
    //ftell - returns the current value of the file position indicator for the stream

    expirement_tell(file);
    expirement_seek(file);
    
    more_seek(file);




    file.close();
    return EXIT_SUCCESS;
}

// ---------- Functions ----------
void more_seek(std::fstream& file)
{
  
    char word1, word2, word3, word4;
    
    file.seekg(0, std::ios_base::beg); // <- seek from beginning
    file.get(word1);
    // get make seekg move to +1
    // so we are currently at the second char b

    file.seekg(1, std::ios_base::cur); // -> seek from cur pos toward the end
    file.get(word2);
    //we currently on d
   
    file.seekg(-1, std::ios_base::cur); // <- seek from cur pos (end) toward
    //if we extend outside we will get -1
    cout << "The position of the pointer seekg(-6, std::ios_base::cur) is: " << file.tellg() << endl;
     file.get(word3); 


    //if we want the 6th char from the end
    file.seekg(0, std::ios_base::end); // <- seek from end toward
    file.seekg(-1, std::ios_base::end); // <- seek from end toward 
    cout << "The position of the pointer seekg(-6, std::ios_base::end) is: " << file.tellg() << endl;
        file.get(word4);


    cout << "The first char is: " << word1 << endl;
    cout << "The second char is: " << word2 << endl;
    cout << "The -6 position char from cur pos: " << word3 << endl;
    cout << "The -6 position char from end: " << word4 << endl;
    
}
void expirement_seek(std::fstream& file)
{
    //seekg - sets the file position indicator for the input stream
    //because we know the last position is -1 
    // cause we used tellg
    // we know we need to return to the beginning of the file
    // so we will use std::ios::beg
    cout << "our pointer is at : " << file.tellg() << endl;
    if (file.tellg() != 0)
    {
        cout << "The pointer is at the end of the file" << endl;
        file.seekg(0, std::ios::beg);
        cout << "The pointer after seekg is at " << file.tellg() << endl;
    }


    cout << "printing the file 3 times" << endl;
    int times = 3;
    for (int i = 0; i < times; i++)
    {
      while(file.tellg() != -1)
        {
           
            char c = file.get();
            cout << c;
        }
        file.clear();
        file.seekg(0, std::ios::beg);
        cout << endl;
    }
    
}
void expirement_tell(std::fstream& file)
{
    //tellg - returns the current position of the get pointer
    //in the input stream
    //tellp - returns the current position of the put pointer
    //in the output stream
  
    //this will return the position of the get pointer
    while(!file.eof())
    {
          int position = file.tellg();
    cout << "The position of the tellg pointer is: " << position << endl;
    char c = file.get();
    }
    int position = file.tellg();
    file.clear();
    cout << "The last position of the tellg pointer is: " << position << endl;
    //curenntly the position is at the end of the file
    //so the position will be -1
  
    //this will return the position of the put pointer

}