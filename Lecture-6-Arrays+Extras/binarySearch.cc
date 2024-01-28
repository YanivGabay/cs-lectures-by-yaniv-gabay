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
   
    int students_id[CLASS_SIZE];
    int wanted_id = 0;
    int stud = 0;
    // read in the grades
    // we make sure for each student, that the grade is between 0 and 100,if not
    // we take another cin for that exact same cell
    int found = false;
    int low = 0;
    int mid = 0;
    int high = CLASS_SIZE - 1;//why????


    //low mid and high are indexes of the array

    // this example will work only if the array
    // is sorted!!!
    for (stud = 0; stud < CLASS_SIZE; stud++)
    {
        students_id[stud] = stud * 2;
        // we fill it with 0 2 4 6 8
    }
    std::cout << "students_ids:" << std::endl;

     for (stud = 0; stud < CLASS_SIZE; stud++)
    {
        std::cout << students_id[stud] <<" ";
        // we fill it with 0 2 4 6 8
    }
    std::cout << std::endl <<"Enter wanted id: ";
    std::cin >> wanted_id;

    while (low <= high && !found)
    {
        mid = (low + high) / 2;
        if (students_id[mid] == wanted_id)
        {
             found = true;
             std::cout << "wanted id is in index: " << mid << std::endl;
             std::cout << "value is: " << students_id[mid] << std::endl;
        }
           
        else if (students_id[mid] < wanted_id)
            low = mid + 1;
        else
            high = mid - 1;
    }
    if (found)
        cout << "In list" << endl;
    else
        cout << "Not in list\n";
    return 0;
}
