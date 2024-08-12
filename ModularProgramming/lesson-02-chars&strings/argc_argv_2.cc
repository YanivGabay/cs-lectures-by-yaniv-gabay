/********************************************************************
 * Course: Modolar Programming in C++
 * Lecture: 2 - basic string
 * File: basic_string_c_bonus.cc
 * 
 * Author: Yaniv Gabay
 * Date: 2024-04-08
 * 
 * Overview:
 * lets see some usage of argc and argv
 * for our current tools, this can help us make some kind of 
 * console programs, where we take the input while running the program.
 * so this is the Add program, it will take two numbers and print the sum
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
const int MAX_STRING_SIZE = 100;
const int MAX_STRINGS_ALLOWED = 20;

const int WANTED_ARGUMENTS = 3;
// ---------- Function Prototypes ----------
bool checkInput(char* argv[]);
// ---------- Main Function ----------
int main(int argc, char* argv[]) {

    //we know we got a bad number of input from the user
    if(argc != WANTED_ARGUMENTS){
        std::cerr << "Bad arguemnts , The Usage: " << argv[0] << " <number1> <number2>" << endl;
        // you will learn about cerr in the future
        // but for now, just know that it is like cout, but for errors
        return 1;
    }
    cout << "welcome to the add program" << endl;
    if(checkInput(argv))
    {
    cout << "The sum of " << argv[1] << " and " << argv[2] << " is: " << atoi(argv[1]) + atoi(argv[2]) << endl;
    return 0;
    }

    cout << "something went wrong, please enter numbers only, you can run the program again, bye 3>";
    return 1;



    
}

// ---------- Functions ----------
bool checkInput(char* argv[]){
    for(int i = 1; i < WANTED_ARGUMENTS; i++){
        for(int j = 0; j < strlen(argv[i]); j++){
            if(!isdigit(argv[i][j])){
                return false;
            }
        }
    }
    return true;
}
