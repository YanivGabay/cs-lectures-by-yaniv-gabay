/********************************************************************
 * Course: Modular Programming in C++
 * Lecture: 4 - Files
 * File: 2_basic_files.cc
 *
 * Author: Yaniv Gabay
 * Date: 2024-04-26
 *
 * Overview:
 * files operations.
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
void closeFiles(std::ifstream &file1, std::ifstream &file2);
void takeInput(char user_input[], char user_input2[]);
bool openFiles(std::ifstream &file1, std::ifstream &file2,
               const char file1_name[], const char file2_name[]);

void printCat();
void handleFiles(std::ifstream &file1, std::ifstream &file2);
bool check_eof_files(std::ifstream &file1, std::ifstream &file2);
char read_next_char(std::ifstream &file1, std::ifstream &file2, const bool turn);
// ---------- Main Function ----------
int main()
{

    char user_input[MAX_SIZE] = {0};
    char user_input2[MAX_SIZE] = {0};

    cin >> std::setw(MAX_SIZE) >> user_input;
    cin >> std::setw(MAX_SIZE) >> user_input2;

    std::ifstream file1, file2;

    if (!openFiles(file1, file2, user_input, user_input2))
    // means if openFiles returned false
    {
        // here we can do more actions
        // if needed , you will see this in the future
        cout << "Exiting the program cause of an error. \n";

        return EXIT_FAILURE;
    }
    // some manipulation
    handleFiles(file1, file2);

    closeFiles(file1, file2);

    return EXIT_SUCCESS;
}

// ---------- Functions ----------
void handleFiles(std::ifstream &file1, std::ifstream &file2)
{
    cout << "Files are open" << endl;
    cout << "Printing one char from each file\n";

    char ch1, ch2;
    bool turn = true;
    while (true)
    {
        if (check_eof_files(file1, file2))
        {
            break;
        }

        cout << read_next_char(file1, file2, turn);
        turn = !turn;
    }
    cout << endl;
}
bool check_eof_files(std::ifstream &file1, std::ifstream &file2)
{
    if (file1.eof() || file2.eof())
    {
        cout << endl;
        cout << "One of the files are empty\n";
        return true;
    }
    return false;
}
char read_next_char(std::ifstream &file1, std::ifstream &file2, const bool turn)
{
    // this is tricky
    char ch = 0;

    std::ifstream &selected_file = turn ? file1 : file2;

    selected_file.get(ch);
    if(isspace(ch))
    ch =0;//empty char ascii value

    return ch;
   
}
void closeFiles(std::ifstream &file1, std::ifstream &file2)
{
    file1.close();
    file2.close();
}
void takeInput(char user_input[], char user_input2[])
{

    // can also use:
    //  cin >> setw(MAX_SIZE) >> user_input;

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
    // can also do:
    //  file1(file1_name) and file2(file2_name)// and they will open the files

    if (file1.is_open() && file2.is_open())

    // can also do: if(file1 && file2)
    {
        cout << "Files are open, good (:\n";
        return true;
    }
    else
    {
        std::cerr << "Files didnt open sorry ):\n";
        std::cerr << "Path of files: " << file1_name << " and " << file2_name << endl;
        return false;
    }
}