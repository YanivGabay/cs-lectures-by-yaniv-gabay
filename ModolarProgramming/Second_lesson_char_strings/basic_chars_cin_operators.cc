/********************************************************************
 * Course: Modolar Programming in C++
 * Lecture: 2 - chars and strings
 * File: basic_chars_cin_operators.cc
 * 
 * Author: Yaniv Gabay
 * Date: 2024-04-08
 * 
 * Overview:
 * This program need to take a input contains of several chars , convert each char to a number,
 * we want to print the average of the int value of the chars
 * 
 * 
 * Input example:
 * 4   // means we need to read 4 lines
 * a a a        
 * b H        b     ///can have multiply spaces between them
 * c                 //different length aswell
 * d d d     d      ///yes
 * // pay attention that the input can have multiply spaces between the chars 
 * // and we use new line after each row!! pressing enter if you like
 * 
 * Output example:
 * using ascii values (a+a+a)/3 = 97+97+97/3 = 97
 *  (b+H+b)/3 = (98+72+98)/3 = 89
 *  c = 99    
 *  (d+d+d+d)/4 = 100+100+100+100/4 = 100
 * 
 * 
 * Notes:
 * Any additional notes related to this file.
 *******************************************************************/

// ---------- Include Section ----------
#include <iostream>
// Add more #include directives as needed

// ---------- Using Section ----------
using std::cout;
using std::cin;
using std::endl;

// ---------- Constants ----------
const int MAX_SIZE = 100;


// ---------- Function Prototypes ----------
double getAverage() ;

// ---------- Main Function ----------
int main() {

    

    //this array just for ourself to store the results and maybe
    //use it later
    double results[MAX_SIZE];


    //how many rows == the first number inside the input 
    int num_of_rows = 0;

    // just an example of showing cin.peek() usage
    // if we want to check about the current char in the input
    // without "eating" it, we can use cin.peek()

    char c = cin.peek();
    std::cout << "The next char in the input is: " << c << std::endl;

    std::cin >> num_of_rows;
    std::cout << "The number of rows is: " << num_of_rows << std::endl;
    std::cin.ignore(); // we need to ignore the new line char

    for(int i = 0; i < num_of_rows; i++)
    {
        std::cout << "Enter values for this row,no matter how many spaces" << i+1 << " is: " << std::endl;
        results[i] = getAverage();
        printAverage(i, results[i]);
     
    }

    return 0;
}

// ---------- Functions ----------
void printAverage(int index, double average)
{
       std::cout << "=============================" << std::endl;
        std::cout << "The average of the row number " << index+1 << " is: " << average << std::endl;
        std::cout << "=============================" << std::endl;
}
double getAverage() {

    int num = 1;
    int sum = 0;
    int temp = 0;
    char single_char;

    while (true)
    {   
        //we can get a line starting with space, so we always here 
        //skip those spaces by eating them
        do
        {
            single_char = std::cin.get();
        } while (single_char == ' ');

        //if we got a new line, we can break the loop
        if(single_char == '\n')
        {
            return double(sum / num);
        }
        //if we reach here, we got a char that isnt new line
        // and isnt space, so we can convert it to int
        std::cout << "The char is: " << single_char << std::endl;
        std::cout << "The ascii value of"<< single_char << "the char is: " << int(single_char) << std::endl;
        temp = single_char;
        sum += temp;
        num++;

    }
    


}
