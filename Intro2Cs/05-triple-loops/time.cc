///////------//lecture 5 - triple loops - time example//------///////

/*in this example, we will show a usage of triple nested loops
   to display all the possible times in a day in HH:MM:SS format.
   this is using for loops, but you can make the same print using while loops.
   */

//include section
#include <iostream>
#include <iomanip> // For std::setw

//main section
int main()
{
    // variables section
    int hoursLimit = 24;
    int minutesLimit = 60;
    int secondsLimit = 60;

    //triple nested loop to print all the possible times in a day

    // can run from 0 to 23
    for (int hour = 0; hour < hoursLimit; hour++)
    {
        // can run from 0 to 59 for each hour
        for (int minute = 0; minute < minutesLimit; minute++)
        {
            // can run from 0 to 59 for each minute
            for (int second = 0; second < secondsLimit; second++)
            {
                // prints: "hour:minute:second" endl
                std::cout << hour << ":"
                          << minute << ":"
                          << second << std::endl;
            }
        }
    }
    /*what we can learn from the output of this program is that
    the rightest numbers, "change" the most, and we can see that its almost looks
    like a premant rolling. the leftest numbers, "change" the least,
     and we can barley see them change    */
    // the for loops for hours, - least amount of cycles
    // the for loops for minutes, - medium amount of cycles
    // the for loops for seconds, - most amount of cycles by far.

    return 0;
}
