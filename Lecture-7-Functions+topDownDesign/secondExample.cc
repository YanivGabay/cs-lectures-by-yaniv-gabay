
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
double calculateAverageReturn(double arr[]);
void calculateAveragePrint(double arr[]);
void calculateAverageReference(double arr[], double &avg);
// using section
using ::std::cin;
using ::std::cout;
using ::std::endl;
//main section
int main()
{
    double numbers[SIZE] = {5, 10, 15, 20, 25}; // Initialize an array of integers

    // Using the function with return type
    double average = calculateAverageReturn(numbers);
    cout << "Average (Return): " << average << endl;

    // Using the void function
    calculateAveragePrint(numbers);

    // Using the function with reference parameter
    double avgRef = 0; // This will be updated by the function
    calculateAverageReference(numbers, avgRef);
    cout << "Average (Reference): " << avgRef << endl;


    return (EXIT_SUCCESS);

}
//---------------------------------------------
/////// pay attention, that we using the SIZE variable as the loop condition
/////// if we wanted a general function, to work on any array, we would need to get 
////// the size of the array as a parameter

// Function with return type
double calculateAverageReturn(double arr[]) {
    int sum = 0;
    for (int i = 0; i < SIZE; i++) {
        sum += arr[i];
    }
    return (sum) / SIZE;
}
//---------------------------------------------
// Void function
void calculateAveragePrint(double arr[]) {
    int sum = 0;
    for (int i = 0; i < SIZE; i++) {
        sum += arr[i];
    }
    cout << "Average (Print inside function): " << (sum) / SIZE << endl;
}
//---------------------------------------------
// Function modifying a reference parameter
void calculateAverageReference(double arr[], double &avg) {
    int sum = 0;
    for (int i = 0; i < SIZE; i++) {
        sum += arr[i];
    }
    avg = (sum) / SIZE;
}