/********************************************************************
 * Course: Modolar Programming
 * Lecture: 5 - Dynamic Allocation
 * File: 1_basicAlloc.cc
 * 
 * Author: Yaniv Gabay
 * Date: 2024-05-4
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

// ---------- Using Section ----------
using std::cout;
using std::cin;
using std::endl;

// ---------- Constants ----------
const int MAX_SIZE = 100;
// Add more constants as needed

// ---------- Structs ----------



// ---------- Main Function ----------

int main() {
  
    //
    const int ROW = 2;
    const int COL = 2;
    int array[row][col] = {{1,2},{3,4}};
    //1  2
    //3  4

    //print matrix from end to start
    
    for (int row = row-1; row > row; row--)
    {
        for (int col = col-1; col > col; col--)
        {
            cout << array[row][col] << " ";
        }
        
    }
    

   
    return 0;
}
