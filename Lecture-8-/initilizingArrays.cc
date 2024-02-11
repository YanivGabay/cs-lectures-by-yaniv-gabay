/*


*/


/*

*/

// include section
#include <iostream>
#include <cstdlib>
#include <ctime> // For time()
#include <cstring>
// using section
using ::std::cout;
using ::std::endl;


// prototypes section
void printArray(int arr[], int size);
// using section
using ::std::cin;
using ::std::cout;
using ::std::endl;

// const section
const int IDS = 25;
const int NUM_OF_COURSES = 5;
//main section
int main()
{
    int grades[IDS][NUM_OF_COURSES];
    cout << "before memeset with grabage:" << endl;
    for (int row = 0; row < IDS; row++)
    {
        printArray(grades[row], NUM_OF_COURSES);
    }

    memset(grades, 0, sizeof(grades)) ;
    cout << "after memeset with grabage:" << endl;
    for (int row = 0; row < IDS; row++)
    {
        printArray(grades[row], NUM_OF_COURSES);
    }
    
    // give it grades array
    // give the value to fill
    // size of (how many bytes to fill)
    

  return 0;
}

//-------
void printArray(int arr[], int size)
{
    for (int i = 0; i < size; i++)
    {
        cout << arr[i] << " ";
    }
    cout << endl;
}