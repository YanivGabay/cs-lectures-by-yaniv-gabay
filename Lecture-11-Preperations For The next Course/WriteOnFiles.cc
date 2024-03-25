/********************************************************************
 * Course: Modolar Programming in C++
 * Lecture: 1 - Files
 * File: WriteOnFiles.cc
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
void writeFromFunction(std::ofstream &output_fd) ;
// Add more function prototypes as needed

// ---------- Main Function ----------
int main() {
    // Main function code
    
    std::ofstream output_fd;
    //output stream not input stream

    

    // open the file
    //1.
        //can also use from input 
        // char file_name[MAX_LENGTH];
       // cin >> std::setw(MAX_LENGTH) >> file_name;
       // output_fd.open(file_name);
    //or             
    output_fd.open("output.txt");
   
    //will create that file if its doesnt exists
    //output_fd.open("output.txt",std::ios::app);
    //2. after we run the first time, we will change to this:

    //we use this: so we can append to the end of the file
    //
    //lets say its exists, but we want to write on exists
    // so if the file has: "1  2   3"
    // and we will write "4 5 6"
    // the file will be: "1  2   3 4 5 6"

    
    

    if(!output_fd.is_open()) {
        std::cerr << "Error opening file" << endl;
        return EXIT_FAILURE;
    }

    // ifstream for reading file
    // cin >> number || input_fd >> number

    // ofstream for writing to file

    //now we will write to the file
    // by using the same syntax as cout
    output_fd << "Hello World" << endl; // cout << "Hello World" << endl;
    output_fd << "This is a test from main" << endl;
    output_fd << "Goodbye" << endl;

    //always send with & when parameter is the 
    // file descriptor
    
    writeFromFunction(output_fd);






    //there is the thing called the buffer
    //it is a temporary storage area
    //when we write to a file, we write to the buffer
    //and when the buffer is full, it writes to the file
    //so if we want to write to the file immediately
    //we can use this:
    output_fd.flush();
    //this will write to the file immediately
    //what ever is in the buffer
    
    //like we can use the close function
    //to close the file
    output_fd.close();
   

    return 0;
}

// ---------- Functions ----------
void writeFromFunction(std::ofstream &output_fd) {
   
    output_fd << "Hello World" << endl;
    output_fd << "This is a test from function" << endl;
    output_fd << "Goodbye" << endl;
        
}
// Add more functions as needed