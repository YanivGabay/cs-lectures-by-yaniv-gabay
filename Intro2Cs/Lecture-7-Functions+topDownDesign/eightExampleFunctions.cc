
///////------//lecture 7 - intro to functions//------///////

/*
    we will create togther using top down design
    a program that will verify if two arrays contain the same
    numbers, without considering the order of the numbers

    for this example we will create a bool array
    and for each number check if the other is in the array
    if so, we will put true on the original place location
    it would be better to just sort them both, instead of doing n^2 work
    but this is just for learning purposes
*/

// include section
#include <iostream>
#include <cstdlib>
#include <ctime> // For time()

// using section
using ::std::cout;
using ::std::endl;
// const section
const int SIZE = 6;
// prototypes section


// using section
using ::std::cin;
using ::std::cout;
using ::std::endl;
//main section
int main()
{
    int random_array[] = getRandomArray();
    return (EXIT_SUCCESS);

}
//function section
int[] getArray()
{
    int array[SIZE] = {0};
    for (int i = 0; i < SIZE; i++)
    {
        array[i] = rand() % 10;
    }
    return array;
}
