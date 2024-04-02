/********************************************************************
 * Course: Course Name
 * Lecture: Lecture 1 - BackTracking
 * File: premutationsHarderFromLastSemster.cc
 * 
 * Author: Yaniv Gabay
 * Date: 2024-04-02
 * 
 * Overview:
 * Brief description of the file's purpose.
 * 
 * Notes:
 * Any additional notes related to this file.
 *******************************************************************/

// ---------- Include Section ----------
#include <iostream>
// Add more #include directives as needed

// ---------- Using Section ----------
using std::cout;
using std::cin;
using std::endl;

// ---------- Constants ----------
const int SIZE = 3;
// ---------- Structs ----------
struct ExampleStruct {
    int exampleMember;
};
// Add more structs as needed

// ---------- Function Prototypes ----------
void swap(int &a, int &b);
void generatePermutations(int arr[], int startIndex, int endIndex);

// ---------- Main Function ----------
int main()
{
    int arr[] = {1, 2, 3};

    std::cout << "Permutations of the array: " << std::endl;
    generatePermutations(arr, 0, SIZE - 1);

    return 0;
}

// ---------- Functions ----------
void swap(int &a, int &b)
{
    int temp = a;
    a = b;
    b = temp;
}

void generatePermutations(int arr[], int startIndex, int endIndex)
{
    if (startIndex == endIndex)
    {
        // Base case: If startIndex equals endIndex, we have reached a permutation.
        // Print the current permutation.
        for (int i = 0; i <= endIndex; ++i)
            std::cout << arr[i] << " ";
        std::cout << " (Permutation Complete)" << std::endl;
        return;
    }
    else
    {
        // Print the current state of the array
        std::cout << "Current State: ";
        for (int i = 0; i <= endIndex; ++i)
            std::cout << arr[i] << " ";
        std::cout << " (Start Index: " << startIndex << ", End Index: " << endIndex << ")" << std::endl;

        // Recursively generate permutations by swapping elements.
        for (int i = startIndex; i <= endIndex; ++i)
        {
            swap(arr[startIndex], arr[i]); // Swap the current element with the first element

            // Print the swap
            std::cout << "Swapping: " << arr[startIndex] << " and " << arr[i] << std::endl;

            generatePermutations(arr, startIndex + 1, endIndex); // Recur for the remaining elements

            swap(arr[startIndex], arr[i]); // Undo the swap to backtrack

            // Print the backtrack
            std::cout << "Backtracking: " << arr[startIndex] << " and " << arr[i] << std::endl;
        }
    }
}



