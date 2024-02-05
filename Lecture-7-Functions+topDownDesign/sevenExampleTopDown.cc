
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
void fill_array(int user_array[]);

void compare_arrays_result(int original_array[], int user_array[]);

void display_result(bool result);

bool compare_arrays(int original_array[], int user_array[]);

bool check_bool_array(bool check_array[]);

// using section
using ::std::cin;
using ::std::cout;
using ::std::endl;
//main section
int main()
{
   int original_array[SIZE] = {1, 2, 3, 4, 5, 6};
   int user_array[SIZE] = {0};
   //we will need a function 
   //to fill the first array
   fill_array(user_array);
   compare_arrays_result(original_array, user_array);
    return (EXIT_SUCCESS);

}
//function section
//---------------------
void fill_array(int user_array[])
{
    for (int i = 0; i < SIZE; i++)
    {
        cout << "Please enter a number: ";
        cin >> user_array[i];
    }
}
//---------------------
void compare_arrays_result(int original_array[], int user_array[])
{
    
    //we will need a function to compare the arrays
    bool result = compare_arrays(original_array, user_array);
    //we will need a function to display the result
    display_result(result);
}
//---------------------
void display_result(bool result)
{
    if (result)
    {
        cout << "The arrays are the same" << endl;
    }
    else
    {
        cout << "The arrays are different" << endl;
    }
}
//---------------------
bool compare_arrays(int original_array[], int user_array[])
{
    bool check_array[SIZE] = {false};
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            if (original_array[i] == user_array[j])
            {
                check_array[i] = true;
            }
        }
    }

    return check_bool_array(check_array);
   
    
}
//---------------------
bool check_bool_array(bool check_array[])
{
    for (int i = 0; i < SIZE; i++)
    {
        if (check_array[i] == false)
        {
            return false;
        }
    }
    return true;
}
