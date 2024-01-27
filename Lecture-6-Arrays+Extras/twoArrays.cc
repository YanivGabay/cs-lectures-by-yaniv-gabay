///////------//lecture 6 - basicRuns on arrays//------///////

/*
   complex run with two arrays
   to find the highest grades
   using a second array
   we save steps on second run for the equals
   good game to understand indexs
*/

// include section
#include <iostream>
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
    // find highest grade
    int max = -1; // why -1?????? everyone got 0
    int grades[CLASS_SIZE];
    int best_stud_indexes[CLASS_SIZE];
    int stud = 0;
    // read in the grades
    // we make sure for each student, that the grade is between 0 and 100,if not
    // we take another cin for that exact same cell

    int occupy = 1;

    for (stud = 0; stud < CLASS_SIZE; stud++)
    {
        do
        {
            std::cin >> grades[stud];
        } while (grades[stud] < MIN_GRADE || grades[stud] > MAX_GRADE);
    }

    for (stud = 0; stud < CLASS_SIZE; stud++)
    {
        if (grades[stud] > max) // a new best stud is found
        {
            //first value ALWAYS comes in here first time of the loop
            max = grades[stud];
            best_stud_indexes[0] = stud; // only stud is best,
            // as far as we know
            occupy = 1; // first one !!
        }
        else if (grades[stud] == max) // additional best stud
        {
            best_stud_indexes[occupy] = stud;
            occupy++;
        }
    }
    cout << max << endl;
    // display the best studs
    for (stud = 0; stud < occupy; stud++)
        cout << best_stud_indexes[stud] << " ";
    cout << endl;
    return 0;
}
