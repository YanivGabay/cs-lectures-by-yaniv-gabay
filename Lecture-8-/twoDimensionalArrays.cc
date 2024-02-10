/*


*/


/*

*/

// include section
#include <iostream>
#include <cstdlib>
#include <ctime> // For time()

// using section
using ::std::cout;
using ::std::endl;


// prototypes section

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


    for (int id = 0; id < IDS; id++)
    {
        for (int course = 0; course < NUM_OF_COURSES; course++)
        {
            grades[id][course] = 0;
        }
        
    }


    std::cout << "Printing the 2D array:" << std::endl;
      for (int id = 0; id < IDS; id++)
    {
        for (int course = 0; course < NUM_OF_COURSES; course++)
        {
             std::cout <<  grades[id][course] << "\t";
        }
         std::cout << std::endl;
    }
    
    
    for (int id = 0; id < IDS; id++)
    {
        grades[id][0] = rand() % 100000;
        
    }

     std::cout << "Printing the 2D array with id:" << std::endl;
      for (int id = 0; id < IDS; id++)
    {
        for (int course = 0; course < NUM_OF_COURSES; course++)
        {
             std::cout <<  grades[id][course] << "\t";
        }
         std::cout << std::endl;
    }
    


      for (int id = 0; id < IDS; id++)
    {
        for (int course = 1; course < NUM_OF_COURSES; course++)
        {
            grades[id][course] = rand() % 100;
        }
        
    }

      std::cout << "Printing the 2D array with grades:" << std::endl;
      for (int id = 0; id < IDS; id++)
    {
        for (int course = 0; course < NUM_OF_COURSES; course++)
        {
             std::cout <<  grades[id][course] << "\t";
        }
         std::cout << std::endl;
    }
    

  return 0;
}