///////------//lecture 6 - basicRuns on arrays//------///////

/*
    basic runs on loops
    yorams for nested while for inserting input
    find highest grade very basic
    must think if there are multiply equals
*/

// include section
#include <iostream>
// using
using ::std::cout;
using ::std::endl;

// const section
const int CLASS_SIZE = 10;
// main section
int main()
{
    // find highest grade
    int max = -1;
    int grades[CLASS_SIZE];
    int stud = 0;
    // read in the grades
    // we make sure for each student, that the grade is between 0 and 100,if not
    // we take another cin for that exact same cell
    for (stud = 0; stud < 40; stud++)
    {
        do
        {
            std::cin >> grades[stud];
        } while (grades[stud] < 0 || grades[stud] > 100);
    }
    // find the highest grade
    for (int stud = 0; stud < CLASS_SIZE; stud++)
        if (grades[stud] > max)
            max = grades[stud];
    cout << max << endl;

    // find the students that share the highest grade
    for (stud = 0; stud < CLASS_SIZE; stud++)
        if (grades[stud] == max)
            cout << stud << " ";

    return 0;
}
