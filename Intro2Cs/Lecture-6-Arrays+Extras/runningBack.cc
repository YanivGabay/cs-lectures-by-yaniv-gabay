///////------//lecture 6 - runningBackwards on arrays//------///////


/*
   
*/

// include section
#include <iostream>
// using
using ::std::cout;
using ::std::endl;

// const section
const int CLASS_SIZE = 5;
const int NUMBERS_SIZE = 20;
// main section
int main()
{
    // find highest grade
    
    int grades[CLASS_SIZE];
    int stud = 0;
    // read in the grades
    // we make sure for each student, that the grade is between 0 and 100,if not
    // we take another cin for that exact same cell
    for (stud = 0; stud < CLASS_SIZE; stud++)
    {
        do
        {
            std::cin >> grades[stud];
        } while (grades[stud] < 0 || grades[stud] > 100);
    }
    cout << "print from end:" << endl;
    //remember, if size of array is 5, then the last index is 4
    //print from end
    for (stud = CLASS_SIZE-1; stud >= 0; stud--)
        cout << grades[stud] << " ";

    cout << endl;
    int numbers[NUMBERS_SIZE];
    //print one from one for end one from start
    for (int number = 0; number < NUMBERS_SIZE; number++)
    {
        numbers[number] = 2*number;
    }
    cout << "printing regular new array:" << endl;
      for (int number = 0; number < NUMBERS_SIZE; number++)
    {
        cout << numbers[number] << " ";
       
    }
      cout <<  endl  <<"printing 1 front 1 back new array:" << endl;
    for (int number = 0; number < NUMBERS_SIZE/2; number++)
    {
        cout << numbers[number] << " ";
        cout << numbers[NUMBERS_SIZE-number-1] << " ";
    }

    return 0;
}
