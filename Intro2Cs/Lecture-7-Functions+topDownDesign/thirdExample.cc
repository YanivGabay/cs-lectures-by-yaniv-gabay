
///////------//lecture 7 - intro to functions//------///////

/*
   this examples will show 3 variants of functions:
  # void without return value or modify value inside function
  #  int with return value
   # void with modify value inside function
   // but this time, its calculting the avg of an array
*/
// its very important to understand this concept, as it will be used in the next lectures


// include section
#include <iostream>
#include <cstdlib>
using ::std::cout;
using ::std::endl;
// const section
const int SIZE = 5; // Constant for the array size
// prototypes section
double calculateSum(double arr[]);
double calculateAverage(double arr[], double sum);
double findMaximum(double arr[]);
// using section
using ::std::cin;
using ::std::cout;
using ::std::endl;
//main section
int main()
{
  
 double numbers[SIZE] = {5, 10, 15, 20, 25}; // Initialize an array of integers

    // Calculate the sum of array elements
    double sum = calculateSum(numbers);
    cout << "Sum: " << sum << endl;

    // Calculate the average of array elements
    double average = calculateAverage(numbers, sum);
    cout << "Average: " << average << endl;

    // Find the maximum number in the array
    double max = findMaximum(numbers);
    cout << "Maximum: " << max << endl;

    return (EXIT_SUCCESS);

}
//---------------------------------------------
/////// pay attention, that we using the SIZE variable as the loop condition
/////// if we wanted a general function, to work on any array, we would need to get 
////// the size of the array as a parameter



// Function to calculate sum of array elements
double calculateSum(double arr[]) {
    int sum = 0;
    for (int i = 0; i < SIZE; i++) {
        sum += arr[i];
    }
    return sum;
}

// Function to calculate average of array elements
double calculateAverage(double arr[], double sum) {
    return (sum) / SIZE;
}

// Function to find the maximum number in the array
double findMaximum(double arr[]) {
    int max = arr[0];
    for (int i = 1; i < SIZE; i++) {
        if (arr[i] > max) {
            max = arr[i];
        }
    }
    return max;
}