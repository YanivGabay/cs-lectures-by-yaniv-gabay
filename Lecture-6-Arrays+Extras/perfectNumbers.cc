
///////------//lecture 6 - bubblesort visualisation on arrays//------///////

/*
  bubble sort
  with visualisation
  you dont need to understand this code
  its only for us to print
*/

// include section
#include <iostream>
#include <cstdlib>
using ::std::cout;
using ::std::endl;
// const section

 const int ARRAY_SIZE = 6;

int main()
{
   
    int numbers[ARRAY_SIZE] = {6, 28, 496, 8128, 33550336, 12}; // Including some known perfect numbers and a non-perfect number
    bool isPerfect[ARRAY_SIZE] = {false};                       // Initialize all elements to false
    //is an bool index array, where is true, means the original array 
    //in the same index is a perfect number
    
    // Check each number in the array for being a perfect number
    for (int i = 0; i < ARRAY_SIZE; ++i)
    {
        int sum = 0; // To store the sum of divisors
        for (int div = 1; div <= numbers[i] / 2; ++div)
        {
            if (numbers[i] % div == 0)
            {
                sum += div; // Add divisor to sum
            }
        }

        // If the sum of divisors equals the number, it's a perfect number
        if (sum == numbers[i])
        {
            isPerfect[i] = true;
        }
    }

    // Print the results
    std::cout << "Perfect numbers in the array: ";
    for (int i = 0; i < ARRAY_SIZE; ++i)
    {
        if (isPerfect[i])
        {
            std::cout << numbers[i] << " ";
        }
    }
    std::cout << std::endl;

    return 0;
}
