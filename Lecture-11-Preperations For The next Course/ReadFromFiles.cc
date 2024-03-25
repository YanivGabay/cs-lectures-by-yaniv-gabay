/********************************************************************
 * Course: Modolar Programming in C++
 * Lecture: 1 - Files
 * File: ReadFromFiles.cc
 * 
 * Author: Yaniv Gabay
 * Date: 2024-03-24
 * 
 * Overview:
 * Show some examples of using files
 * 
 * Notes:
 * 
 *******************************************************************/

// ---------- Include Section ----------
#include <iostream>
// Add more #include directives as needed
#include <fstream> // new include !!!!!!!!!
#include <cctype> // new include !!!!!!!!!
#include <iomanip>// new include !!!!!!!!!
// ---------- Using Section ----------
using std::cout;
using std::cin;
using std::endl;

// ---------- Constants ----------
const int MAX_LENGTH = 100;
// Add more constants as needed

// ---------- Structs ----------

// Add more structs as needed

// ---------- Function Prototypes ----------
void exampleFunction(int parameter);
// Add more function prototypes as needed

// ---------- Main Function ----------
int main() {
    // Main function code
    
    
    std::ifstream input_fd;//reading from files
    std::ofstream output_fd;//writing to files

    //char file_name[MAX_LENGTH];
    // get the file name from the user
    //cin >> std::setw(MAX_LENGTH) >> file_name;
    
    // open the file
    //input_fd.open(file_name);
    
    //we could also directly do 
    input_fd.open("test.txt");
    //important concept through the course
    //when doing file operations, we always need to check if the file is open
    //simillar operations you will learn in the future will always
    //be needed to check if properly done


    //cerr, is like cout, but it is used for errors!!!
    //important to use it when we have specific errors!!

    if(!input_fd.is_open()) {
        std::cerr << "Error opening file" << endl;
        return EXIT_FAILURE;
    }
    
    // read the file
    int number;
    // read the file until the end
    // the input_fd.eof() will return true when we reach the end of the file
    // input_fd.good() 
    // input_fd.bad() 
    // input_fd.fail()
    // input_fd.eof()
    // end of file 
    while(!input_fd.eof()) {
        input_fd >> number; // cin >> number
        cout << number << endl;
    }


    // important, before we finish what we OPEN
    // we need to close it
   
    input_fd.close();

    return 0;
}

// ---------- Functions ----------
void exampleFunction(int parameter) {
    // Example function implementation
    cout << "Parameter value: " << parameter << endl;
}
// Add more functions as needed