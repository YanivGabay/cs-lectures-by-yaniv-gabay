/********************************************************************
 * Course: Modolar Programming in C++
 * Lecture: 2 - basic string
 * File: basic_string_c_bonus.cc
 *
 * Author: Yaniv Gabay
 * Date: 2024-04-08
 *
 * Overview:

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
using std::cin;
using std::cout;
using std::endl;

// ---------- Constants ----------

// ---------- Function Prototypes ----------

void removeComments(char *code);
// ---------- Main Function ----------
int main(int argc, char *argv[])
{

    char code[] = "int main() {\n"
                  "   // This is a single-line comment\n"
                  "   printf(\"Hello, World!\\n\"); /* This is a\n"
                  "   multi-line comment */\n"
                  "   return 0;\n"
                  "}\n";

    std::cout << "Original Code:\n"
              << code << std::endl;
    removeComments(code);
    std::cout << "Code without Comments:\n"
              << code << std::endl;

    return 0;
}

// ---------- Functions ----------
void removeComments(char* code) {
    int read = 0, write = 0;
    bool inSingleLineComment = false, inMultiLineComment = false;

    while (code[read] != '\0') {
        if (!inMultiLineComment && code[read] == '/' && code[read + 1] == '/') {
            inSingleLineComment = true;
            read += 2; // Skip the "//"
        } else if (!inSingleLineComment && code[read] == '/' && code[read + 1] == '*') {
            inMultiLineComment = true;
            read += 2; // Skip the "/*"
        } else if (inMultiLineComment && code[read] == '*' && code[read + 1] == '/') {
            inMultiLineComment = false;
            read += 2; // Skip the "*/"
        } else if (inSingleLineComment && code[read] == '\n') {
            inSingleLineComment = false;
            code[write++] = code[read++]; // Keep the newline character
        } else if (!inSingleLineComment && !inMultiLineComment) {
            // Copy non-comment characters
            code[write++] = code[read++];
        } else {
            // Inside comments, just move the read pointer
            read++;
        }
    }
    code[write] = '\0'; // Null-terminate the processed string
}