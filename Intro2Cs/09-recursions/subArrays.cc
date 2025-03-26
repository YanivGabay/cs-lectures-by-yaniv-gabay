#include <iostream>

using std::cout;
using std::endl;

int findSubSeries(int arr[], int size);
int countSeries(int arr[], int size, int index);
int exploreSeries(int arr[], int size, int index, bool isIncreasing);

int main()
{
    int array[] = {9, 8, 6, 4, 1, 3, 6, 4, 5, 9};
    int count = findSubSeries(array, 10);
    cout << "The result is: " << count << endl;
    return 0;
}

int findSubSeries(int arr[], int size)
{
    if (size < 2)
        return 0; // No series possible with fewer than 2 elements

    return countSeries(arr, size, 0); // Start recursion
}

int countSeries(int arr[], int size, int index)
{
    if (index >= size - 1)
        return 0; // Base case: End of array

    if (arr[index] < arr[index + 1])
    {
        // Increasing series detected
        int nextIndex = exploreSeries(arr, size, index, true);
        return 1 + countSeries(arr, size, nextIndex);
    }
    else if (arr[index] > arr[index + 1])
    {
        // Decreasing series detected
        int nextIndex = exploreSeries(arr, size, index, false);
        return 1 + countSeries(arr, size, nextIndex);
    }
   
}

int exploreSeries(int arr[], int size, int index, bool isIncreasing)
{
    if (index >= size - 1)
        return index + 1; // Base case: End of array or series
        // we return the index + 1 so the outer recursion can also skip the series

    if (isIncreasing && arr[index] < arr[index + 1])
        return exploreSeries(arr, size, index + 1, isIncreasing); // Continue increasing series

    if (!isIncreasing && arr[index] > arr[index + 1])
        return exploreSeries(arr, size, index + 1, isIncreasing); // Continue decreasing series

    return index + 1; // End of series
}
