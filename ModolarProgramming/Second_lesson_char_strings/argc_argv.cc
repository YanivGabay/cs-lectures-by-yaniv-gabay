/********************************************************************
 * Course: Modolar Programming in C++
 * Lecture: 2 - basic string
 * File: basic_string_c_bonus.cc
 * 
 * Author: Yaniv Gabay
 * Date: 2024-04-08
 * 
 * Overview:
 * we have talked about how main is a function, cause of course its syntax is
 * int main() , but why there arent arguemnts?
 * so now we will show the only arguemtns the main function can get
 * which are argc and argv
 * 
 * we will write our main function from now on like this:
 * int main(int argc, char* argv[]) // it equal to int main(int argc, char** argv)
 * if you dont know the * is a pointer, and we will talk about it in the next lectures, so dont worry
 * 
 * what the argc and argv are?
 * argc is the number of arguemnts the program got
 * argv is an array of strings, each string is an argument
 * for example: ./a.out hello world
 * argc will be 3
 * argv[0] will be "./a.out"
 * argv[1] will be "hello"
 * argv[2] will be "world"
 * 
 * nice?
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


// ---------- Function Prototypes ----------
void checkArgcArgv(int argc, char* argv[]);
// ---------- Main Function ----------
int main(int argc, char* argv[]) {


    checkArgcArgv(argc, argv);

    return 0;
}

// ---------- Functions ----------
void checkArgcArgv(int argc, char* argv[]){
    cout << "argc: " << argc << endl;
    for(int i = 0; i < argc; i++){
        cout << "argv[" << i << "]: " << argv[i] << endl;
    }
}
