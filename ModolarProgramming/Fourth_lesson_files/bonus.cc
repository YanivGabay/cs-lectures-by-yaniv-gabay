

/********************************************************************
 * Course: Modular Programming in C++
 * Lecture: 4 - Files
 * File: 2_basic_files.cc
 *
 * Author: Yaniv Gabay
 * Date: 2024-04-26
 *
 * Overview:
 * just some out of the box stuff
 * like const of chars
 * const of a two dimension array of chars
 * a function for checking if files opened proprley
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

const char CAT[][MAX_SIZE] = {
    "  /\\_/\\  ",
    " ( o.o ) ",
    "  > ^ <  ",
    "  /\\_/\\  ",
    ""};
//new line to end the array
const char ERROR_MSG[] = "Error opening file: ";
const char FILE_NAME[] = "file1.txt";
// ---------- Function Prototypes ----------

void printCat();
bool test_errors (const int argc,const char user_input[]);
void runner(const bool result, std::ifstream& file);
void cerr_error(const char error_msg[]);
void close_file(std::ifstream& file);


// ---------- Main Function ----------
int main(int argc, char *argv[])
{
    bool result = test_errors( argc,argv[0]);

    char user_input[MAX_SIZE] = "file1.txt";
    std::ifstream file1;
    

    printCat();
    runner(result, file1);

    
    return EXIT_SUCCESS;
}

// ---------- Functions ----------

void handleFile(std::ifstream& file)
{
    cout << "File is open" << endl;
    //some manipulation
}
void cerr_error(const char error_msg[])
{
    std::cerr <<  error_msg << std::endl;
}
void runner(const bool result, std::ifstream& file)
{
    if (!result)
    {
        close_file(file);
        
        exit(EXIT_FAILURE);
    }

    handleFile(file);


   
}
void close_file(std::ifstream& file)
{
    file.close();
}
bool test_errors(const int argc,const char user_input[])
{
    //some kind of check
    // 
    if (strcmp(user_input, FILE_NAME) == 0)
        return true;

    std::cerr << ERROR_MSG << user_input << std::endl;
    // we  fail we return
    return false;
}

void printCat() {
    for (int i = 0; CAT[i][0] != '\0'; ++i) {
        std::cout << CAT[i] << std::endl;
    }
}
