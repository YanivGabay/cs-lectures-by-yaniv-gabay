/********************************************************************
 * Course: Modular Programming in C++
 * Lecture: 4 - Files
 * File: 2_basic_files.cc
 * 
 * Author: Yaniv Gabay
 * Date: 2024-04-26
 * 
 * Overview:
 * we show some basic cin operations,like cin.eof(),
 * it will work with files as well.
 * after that some basic files operations.
 * https://en.cppreference.com/w/cpp/io/basic_istream
 * to see the operations available for cin.
 * Notes:

 *******************************************************************/

// ---------- Include Section ----------
#include <iostream>
// Add more #include directives as needed

#include <fstream>
//can include only some operation
#include <iomanip>
// ---------- Using Section ----------
using std::cout;
using std::cin;
using std::endl;

// ---------- Constants ----------
const int MAX_SIZE = 100;

// Add more constants as needed






// ---------- Function Prototypes ----------
void closeFiles(std::ifstream &file1, std::ifstream &file2);
void takeInput(char user_input[], char user_input2[]);
bool openFiles(std::ifstream &file1, std::ifstream &file2,
               const char file1_name[], const char file2_name[]);
void printCat();
// ---------- Main Function ----------
int main() {
    
    char user_input[MAX_SIZE] = {0};
    char user_input2[MAX_SIZE] = {0};

    takeInput(user_input, user_input2);


    std::ifstream file1,file2;

    if(!openFiles(file1, file2, "file1.txt", "file2.txt"))
    //means if openFiles returned false
    {
        //here we can do more actions
        //if needed
        cout << "Exiting the program cause of an error. \n" ;
        
        return EXIT_FAILURE;
    }
    //some manipulation


    closeFiles(file1, file2);
    

    return EXIT_SUCCESS;
}

// ---------- Functions ----------

void closeFiles(std::ifstream &file1, std::ifstream &file2)
{
    file1.close();
    file2.close();
}
void takeInput(char user_input[], char user_input2[])
{

    //can also use:
    // cin >> setw(MAX_SIZE) >> user_input;

    cout << "Please enter a string: ";
    cin.getline(user_input, MAX_SIZE);

    cout << "Please enter a string: ";
    cin.getline(user_input2, MAX_SIZE);
}

bool openFiles(std::ifstream &file1, std::ifstream &file2,
               const char file1_name[], const char file2_name[])
{
  
    file1.open(file1_name);
    file2.open(file2_name);

    if(file1.is_open() && file2.is_open())
    {
        cout << "Files are open, good (:\n";
        return true;
    }
    else
    {
        std::cerr << "Files didnt open sorry ):\n";
        return false;
    }
}