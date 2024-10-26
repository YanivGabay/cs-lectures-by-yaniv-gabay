
///////------//lecture 7 - intro to functions//------///////

/*
   bubble sort example, we went over on the previous lectures
   now with functions, also implemnted a function 
   for the swap action (pay attention to the & sign)
   why we needed that?
*/

// include section
#include <iostream>
#include <cstdlib>
using ::std::cout;
using ::std::endl;
// const section
const int SIZE = 6;
// prototypes section
void bubbleSort(int arr[]);
double findMedian(int arr[]);
void swap(int &a, int &b);
// using section
using ::std::cin;
using ::std::cout;
using ::std::endl;
//main section
int main()
{
   int numbers[SIZE] = {22, 55, 33, 11, 44, 66}; // Initialize an array of integers

    // Sort the array
    bubbleSort(numbers);
    cout << "Sorted Array: ";
    for (int i = 0; i < SIZE; i++) {
        cout << numbers[i] << " ";
    }
    cout << endl;

    // Find the median of the array
    double median = findMedian(numbers);
    cout << "Median: " << median << endl;
    return (EXIT_SUCCESS);

}
//---------------------------------------------
// Function to sort the array using bubble sort
void bubbleSort(int arr[]) {
    for (int i = 0; i < SIZE - 1; i++) {
        for (int j = 0; j < SIZE - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                // Swap the elements
                swap(arr[j], arr[j + 1]);
                
            }
        }
    }
}
void swap(int &a, int &b)
{
    int temp = a;
    a = b;
    b = temp;
}
// Function to find the median of the array
double findMedian(int arr[]) {
    if (SIZE % 2 == 0) {
        // If the array size is even, median is the average of the two middle elements
        return (arr[(SIZE / 2) - 1] + arr[SIZE / 2]) / 2.0;
    } else {
        // If the array size is odd, median is the middle element
        return arr[SIZE / 2];
    }
}