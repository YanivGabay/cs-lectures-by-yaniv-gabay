/********************************************************************
 * Course: Course Name
 * Lecture: Lecture Number - Lecture Title
 * File: question1.cc
 * 
 * Author: Your Name
 * Date: 2024-08-CURRENT_DAY
 * 
 * Overview:
 * Brief description of the file's purpose.
 * 
 * Notes:
 * Any additional notes related to this file.
 *******************************************************************/

/*

so the first part of the question is to create a function
called reduce, that get a : const char* strings[], and
an int size, and return a ne array that has all the strings concatenated
*/

/*
the second part of the question is to create a function called split
which gets a string, and a delimiter, and return an array of strings
which were split by the delimiter

*/

// ---------- Include Section ----------
#include <iostream>
// Add more #include directives as needed
#include <cstring>
// ---------- Using Section ----------
using std::cout;
using std::cin;
using std::endl;

// ---------- Constants ----------
int const EXAMPLE_SIZE = 8;
// ---------- Structs ----------


// ---------- Function Prototypes ----------
char* reduce(const char* strings[],const int size);
char** split(const char* string,const char delimiter);
// ---------- Main Function ----------
int main() {

//first part of the question
//create a function called reduce
const char* strings[] = {"hello", "world", "this", "is", "a", "test"};

//reduce(strings, 6) should return "helloworldthisisatest"
char* new_string = reduce(strings, 6);

cout << new_string << endl;

// second part of the question is the opposite
// we get a single string, and we need to split it
// via a given delimiter
const char* string = "hello,world,this,is,a,test";  
char delimiter = ',';
//split(string, ',') should return {"hello", "world", "this", "is", "a", "test"}

char** new_array = split(string, delimiter);
if (new_array == nullptr){
    cout << "null pointer" << endl;
    return 0;
}
cout << "new array after split:" << endl;

//THIS LOOP HAS A FIXED SIZE OF 6,
//if you will change the size of the array, you need to change the loop size
for(int i = 0; i < 6; i++){
    cout << new_array[i] << endl;
}

return 0;
}

  

// ---------- Functions ----------
char* reduce(const char* strings[], const int size){
    if (strings == nullptr || size == 0){
        return nullptr;
    }
    //first we need to calculate the size of the new string
    int new_size = 0;
    for(int i = 0; i < size; i++){
        new_size += strlen(strings[i]);
    }
    //we add 1 for the null terminator
    new_size += 1;
    //now we create the new string
    char* new_string = new char[new_size];
    //here you need to check allocation
    //also to std::nothrow

    //now we copy the strings
    int index = 0;
    for(int i = 0; i < size; i++){
        strcpy(new_string + index, strings[i]);
        index += strlen(strings[i]);
    }
    //we add the null terminator
    new_string[new_size - 1] = '\0';
    return new_string;
}

char** split(const char* string,const char delimiter){
    if (string == nullptr || strlen(string) == 0){
        return nullptr;
    }


    //first we need to count the number of strings
    int count = 1;
    for(int i = 0; i < strlen(string); i++){
        if(string[i] == delimiter){
            count++;
        }
    }
    //now we create the new array
    char** new_array = new char*[count];
    //here you need to check allocation
    //also to std::nothrow

    //now we need to copy the strings
    int index = 0;
    int array_index = 0;
    // index is the index of the current string
    // array_index is the index of the new array
    for(int i = 0; i < strlen(string); i++){
        if(string[i] == delimiter){
            //we allocate index + 1 for the null terminator
            new_array[array_index] = new char[index + 1];
            //here you need to check allocation
            //also to std::nothrow
            strncpy(new_array[array_index], string + i - index, index);
            new_array[array_index][index] = '\0';
            //we need to reset the index
            index = 0;
            //we need to increment the array index
            array_index++;
        }else{
            index++;
        }
    }
    //we need to copy the last string
    new_array[array_index] = new char[index + 1];
    //here you need to check allocation
    //also to std::nothrow
    strncpy(new_array[array_index], string + strlen(string) - index, index);
    new_array[array_index][index] = '\0';

    return new_array;
}