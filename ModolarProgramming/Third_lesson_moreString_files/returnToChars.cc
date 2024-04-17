/********************************************************************
 * Course: Modolar Programming in C++
 * Lecture: 3 - Strings
 * File: returnToChars.cc
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

// ---------- Using Section ----------
using std::cout;
using std::cin;
using std::endl;

// ---------- Constants ----------
const int MAX_SIZE = 100;

enum {SEPERATE_WORDS,DONT_SEPERATE_WORDS};

// ---------- Function Prototypes ----------
void deleteCharFromString(char str[], char c);
void printStringArray(const char array[]);
void multiplyChars(char str[],int times);
void multiplyChars(int new_string_index,int original_index,int times, char str[],char result[]);
void trimwithoutSeperateWords(char str[]);
void trimWithSeperateWords(char str[]);
void stringTrimmer(char str[],int seperate_words);
void copyToDifferentArray(char str[],int index,int word_length,char array[][MAX_SIZE],int word_index);
int searchForLastSeqChar(char str[],const int index);
// ---------- Main Function ----------
int main() {
    int value = 5;
    char c = (char)(value+'0');
    cout << c << endl;

    char array[] = "Hellllaaallllaaall";
    char array2[15] = "Hello";
    //strcat(array,c); //this dont work? what we should do?
    //str cat only accept STRINGs, which are chars arrays with \0 at the end
    //we have to convert the char to a string
    char str[2] = {c, '\0'}; // works
    //than
    

    strcat(array2,str);// will work.
    cout << "string before the deletechar function:" << array << endl;
    deleteCharFromString(array,'l');

    cout << "string before the doubling functioin" << array2 << endl;

    multiplyChars(array2,3);
    cout << "======================" << endl;
    char space_seperated_string[] = "my name     is     yaniv gabay  ";
    stringTrimmer(space_seperated_string,SEPERATE_WORDS);
    

    return 0;
}
// ---------- Functions ----------
// =================================================================
void stringTrimmer(char str[],int seperate_words){
   if (seperate_words == SEPERATE_WORDS)
   {
        trimWithSeperateWords(str);
   }
   else 
   {
         trimwithoutSeperateWords(str);
   }
}
// =================================================================    
void trimwithoutSeperateWords(char str[]){
    int index = 0;
    int new_index = 0;
    while(str[index] != '\0'){
        if(str[index] != ' '){
            str[new_index] = str[index];
            new_index++;
        }
        index++;
    }
    str[new_index] = '\0';
    cout << "result of the trim function:" << str << endl;
    printStringArray(str);
}
// =================================================================

//   The line `char space_seperated_string[] = "my name     is     yaniv gabay  ";` is declaring a character array named `space_seperated_string` and initializing it with the string "my name     is     yaniv gabay  ". This string contains multiple spaces between words, and the goal of the program seems to be to trim these extra spaces based on the specified trimming mode (with or without separating words).
char space_seperated_string[] = "my name     is     yaniv gabay  ";
void trimWithSeperateWords(char str[]){
   
    char array[100][MAX_SIZE] = {0}; // Assume there are 100 words maximum
    int index = 0;
    int word_index = 0;

    while(str[index] != '\0'){
        // Move index to the first non-space character
        while(str[index] == ' ' && str[index] != '\0') {
            index++;
        }

        if (str[index] == '\0') break; // Check if it's the end of the string after skipping spaces

        //cout << "index before searchlastseq" << index << endl;
        int nextCharIndex = searchForLastSeqChar(str, index);
       // cout << "index after searchlastseq" << index << endl;
        int word_length = nextCharIndex - index;
       // cout << "word_length = " << word_length << endl;
       // cout << "nextCharIndex = " << nextCharIndex << endl;

        copyToDifferentArray(str, index, word_length, array, word_index);
        word_index++;

       // cout << "nextCharIndex" << nextCharIndex << endl;
        index = nextCharIndex; // Move to the next character after the word ends

        // Ensure no spaces are included at the start of the next word search
        while(str[index] == ' ' && str[index] != '\0') {
            index++;
        }

       // cout << "index = " << index << endl;
        if (str[index] != '\0') {
            cout << "next char to check = " << str[index] << endl;
        }
    }
}

void copyToDifferentArray(char str[], int index, int word_length, char array[][MAX_SIZE], int word_index){
    for(int i = 0; i < word_length; i++){
        array[word_index][i] = str[index + i];
    }
    array[word_index][word_length] = '\0'; // Null terminate the word
    cout << "word[" << word_index << "] = " << array[word_index] << endl;
}

int searchForLastSeqChar(char str[], const int index){
    int temp = index;
    while(str[temp] != ' ' && str[temp] != '\0'){
        temp++;
    }
    return temp; // Return the index just after the last character of the word
}

// =================================================================

//we want to multiply each char in the string
//and make sure it has \0 at the end
void multiplyChars(char str[],int times){

    int original_index = 0;
    int new_string_index = 0;

    char remains[MAX_SIZE] = {0};

    strcpy(remains,str); // save the original string
    char result[MAX_SIZE] = {0};

    while(remains[original_index] != '\0'){

       
        multiplyChars(new_string_index,original_index,times,remains,result);
        result[new_string_index+times] = '\0'; // make sure the string ends with \0

        original_index++;
        new_string_index += times;
        
    }
   
    cout << "result of the double function:" << result << endl;
    printStringArray(result);
}
void multiplyChars(int new_string_index,int original_index,int times, char str[],char result[])
{
     for (int i = 0; i < times; i++)
        {
            result[new_string_index+i] = str[original_index];
        }
}
//unique way to delete a char from a string

// [hel] [l]
// str[replace] = h
// str[to_delete] = h
// str[to_delete] = str[replace] = h
// to_delete = 1
// replace = 1
// str[replace] = e
// str[to_delete] = e
// str[to_delete] = str[replace] = e
// to_delete = 2
// replace = 2
// str[replace] = l
// str[replace] == char_to_delete
// replace = 3
// str[replace] == \0 //exit loop
// str[to_delete] = \0 // to_delete = 2;
// str[0] = h
// str[1] = e
// str[2] = \0

void deleteCharFromString(char str[], char char_to_delete){
    int replace = 0;
    int to_delete = 0;
    while(str[replace] != '\0'){
        if(str[replace] != char_to_delete){
            str[to_delete] = str[replace];
            to_delete++;
        }
        replace++;
    }
    str[to_delete] = '\0';
    cout << "result of the delte function:" << str << endl;
    printStringArray(str);
}
// =================================================================
void printStringArray(const char array[]){
    int index = 0;
    while(array[index] != '\0'){
        cout << "array[" << index << "] = ";
        cout << array[index] << endl;
        index++;
    }
    cout << endl;
}