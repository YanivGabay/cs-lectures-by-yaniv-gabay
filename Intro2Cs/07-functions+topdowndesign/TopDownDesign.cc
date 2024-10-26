/*


Top–down is a programming style, the mainstay of traditional procedural languages,
 in which design begins by specifying complex pieces and then dividing them into successively smaller pieces.
 The technique for writing a program using top–down methods is to write a main procedure that names all the major functions it will need. 
 Later, the programming team looks at the requirements of each of those functions and the process is repeated. 
 These compartmentalized subroutines eventually will perform actions so simple they can be easily and concisely coded.

*/


/*
 this is a small top down design example
 we look for k from each of two arrays
 in n^2, just to practice top down design
 and functions usage
*/

// include section
#include <iostream>
#include <cstdlib>
#include <ctime> // For time()

// using section
using ::std::cout;
using ::std::endl;
// const section
const int SIZE = 9;
// prototypes section

// using section
using ::std::cin;
using ::std::cout;
using ::std::endl;
//main section
int main()
{
  
    // given two arrays, and 1 key value, k must be equal to the sum of two values one from each array

    //we will do it in n^2 time complexity, using two loops.

    int arr1[SIZE] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    int arr2[SIZE] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    int k = 10;

    compare_arrays();

   
    return (EXIT_SUCCESS);

}
// function section
void compare_arrays()
{

    //for each cell of first arrays

    for (int i = 0; i < SIZE; i++)
    {
        //check_curr_sum();
    }


}