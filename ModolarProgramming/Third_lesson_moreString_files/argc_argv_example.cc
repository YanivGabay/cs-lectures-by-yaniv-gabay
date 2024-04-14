/********************************************************************
 * Course: Modolar Programming in C++
 * Lecture: 3 - argc and argv
 * File: argc_argv_example.cc
 * 
 * Author: Yaniv Gabay
 * Date: 2024-04-14
 * 
 * Overview:

 * 
 * 
 * Notes:
 * 
 *******************************************************************/

// ---------- Include Section ----------
#include <iostream>
// Add more #include directives as needed
#include <cstring>
#include <iomanip>
#include <fstream>
// ---------- Using Section ----------
using std::cout;
using std::cin;
using std::endl;
using std::cerr;
// ---------- Constants ----------
const int MAX_SIZE = 100;


// ---------- Function Prototypes ----------
void analysText(std::ifstream& file);
int wordCounter(const char line[]);
int letterCounter(const char line[]);
int digitCounter(const char line[]);
int specialCharCounter(const char line[]);
int spaceCounter(const char line[]);
int biggestWord(const char line[], int current_biggest);
int smallestWord(const char line[], int current_smallest);
// ---------- Main Function ----------
int main(int argc, char* argv[]) {
 
    //we get the file name from the user
    if(argc != 2){
        cerr << "Usage: " << argv[0] << " <file_name>" << endl;
        return 1;
    }
    std::ifstream input_file(argv[1]);
    if(!input_file){
        cerr << "Could not open the file: " << argv[1] << endl;
        return 1;
    }
    

    analysText(input_file);

    input_file.close();

    return 0;
}
// ---------- Functions ----------
// =================================================================
void analysText(std::ifstream& file){
    char line[MAX_SIZE] = {0};
    int line_counter = 0;
    int word_counter = 0;
    int letter_counter = 0;
    int digit_counter = 0;
    int special_char_counter = 0;
    int space_counter = 0;
    int biggest_word = 0;
    int smallest_word = 0;
    int word_length = 0;
  

    while(file.getline(line, MAX_SIZE)){
        cout << "Processing line: " << line << endl;
        cout << "====================" << endl;
        line_counter++;
        word_counter += wordCounter(line);
        letter_counter += letterCounter(line);
        digit_counter += digitCounter(line);
        special_char_counter += specialCharCounter(line);
        space_counter += spaceCounter(line);
        biggest_word = biggestWord(line, biggest_word);
        smallest_word = smallestWord(line, smallest_word);
    }
    cout << "The file has been processed" << endl;
    cout << "The number of lines in the file is: " << line_counter << endl;
    cout << "The number of words in the file is: " << word_counter << endl;
    cout << "The number of letters in the file is: " << letter_counter << endl;
    cout << "The number of digits in the file is: " << digit_counter << endl;
    cout << "The number of special characters in the file is: " << special_char_counter << endl;
    cout << "The number of spaces in the file is: " << space_counter << endl;
    cout << "The biggest word in the file is: " << biggest_word << endl;
    cout << "The smallest word in the file is: " << smallest_word << endl;
}
// =================================================================
int wordCounter(const char line[]){
    int counter = 0;
    bool in_word = false;
    for(int i = 0; i < strlen(line); i++){
        if(isalnum(line[i])){
            if(!in_word){
                counter++;
                in_word = true;
            }
        }else{
            in_word = false;
        }
    }
    return counter;
}
// =================================================================
int letterCounter(const char line[]){
    int counter = 0;
    for(int i = 0; i < strlen(line); i++){
        if(isalpha(line[i])){
            counter++;
        }
    }
    return counter;
}
// =================================================================
int digitCounter(const char line[]){
    int counter = 0;
    for(int i = 0; i < strlen(line); i++){
        if(isdigit(line[i])){
            counter++;
        }
    }
    return counter;
}
// =================================================================
int specialCharCounter(const char line[]){
    int counter = 0;
    for(int i = 0; i < strlen(line); i++){
        if(!isalnum(line[i]) && !isspace(line[i])){
            counter++;
        }
    }
    return counter;
}
// =================================================================
int spaceCounter(const char line[]){
    int counter = 0;
    for(int i = 0; i < strlen(line); i++){
        if(isspace(line[i])){
            counter++;
        }
    }
    return counter;
}
// =================================================================
int biggestWord(const char line[], int current_biggest){
    int biggest = current_biggest;
    int word_length = 0;
    for(int i = 0; i < strlen(line); i++){
        if(isalnum(line[i])){
            word_length++;
        }else{
            if(word_length > biggest){
                biggest = word_length;
            }
            word_length = 0;
        }
    }
    if(word_length > biggest){
        biggest = word_length;
    }
    return biggest;
}
// =================================================================
int smallestWord(const char line[], int current_smallest){
    int smallest = current_smallest;
    int word_length = 0;
    for(int i = 0; i < strlen(line); i++){
        if(isalnum(line[i])){
            word_length++;
        }else{
            if(word_length < smallest || smallest == 0){
                smallest = word_length;
            }
            word_length = 0;
        }
    }
    if(word_length < smallest || smallest == 0){
        smallest = word_length;
    }
    return smallest;
}
