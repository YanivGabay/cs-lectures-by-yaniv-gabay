///////------//lecture 6 - basicRuns on arrays//------///////

/*
  bubble sort

*/

// include section
#include <iostream>
#include <cstdlib>
// using
using ::std::cout;
using ::std::endl;

// const section
const int CLASS_SIZE = 5;
const int MAX_GRADE = 100;
const int MIN_GRADE = 0;
// main section
int main()
{
    
    int students_id[CLASS_SIZE];
    int wanted_id = 0;
    int stud = 0;
    // read in the grades
    // we make sure for each student, that the grade is between 0 and 100,if not
    // we take another cin for that exact same cell
     for (stud = 0; stud < CLASS_SIZE; stud++)
    {
        do
        {
            std::cin >> students_id[stud];
        } while (students_id[stud] < MIN_GRADE || students_id[stud] > MAX_GRADE);
    }

      for (stud = 0; stud < CLASS_SIZE; stud++)
        cout << students_id[stud] << " ";
    std::cout << std::endl  <<"after sort:" << std::endl;

    for (int round = 0; round < CLASS_SIZE - 1; round++)
    {
        for (stud = 0; stud < CLASS_SIZE - 1; stud++)
            if (students_id[stud] > students_id[stud + 1])
            {
                int temp = students_id[stud];
                students_id[stud] = students_id[stud + 1];
                students_id[stud + 1] = temp;
            }
    }

     for (stud = 0; stud < CLASS_SIZE; stud++)
        cout << students_id[stud] << " ";

    return 0;
}
