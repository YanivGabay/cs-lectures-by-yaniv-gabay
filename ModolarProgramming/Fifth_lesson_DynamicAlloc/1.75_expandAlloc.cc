/********************************************************************
 * Course: Modolar Programming
 * Lecture: 5 - Dynamic Allocation
 * File: 1_basicAlloc.cc
 *
 * Author: Yaniv Gabay
 * Date: 2024-05-4
 *
 * Overview:
 *
 *
 * Notes:
 *
 *******************************************************************/

// ---------- Include Section ----------
#include <iostream>
// Add more #include directives as needed
#include <cstring>
// ---------- Using Section ----------
using std::cin;
using std::cout;
using std::endl;

// ---------- Constants ----------
const int MAX_SIZE = 3;
// Add more constants as needed

// ---------- Structs ----------

// ---------- Function Prototypes ----------
int *expandArray(int *array,int &size);
void printArray(int *array, int size);
// ---------- Main Function ----------

int main()
{
    int *array = new int[MAX_SIZE];
    if (array == nullptr)
    {
        cout << "Memory allocation failed" << endl;
        return 1;
    }
    // we can use the array as a normal array
    // we can also use the array as a pointer
    array[0] = 5;
    *(array + 1) = 10;
    cout << array[0] << " " << array[1] << endl;
    int size = MAX_SIZE;
    array = expandArray(array,size);
    
    // lets say we want to expand the array
    // we can use the same pointer
    cout << "The elements are (OUTSIDE THE FUNC FROM MAIN): ";
    printArray(array, size);
    return 0;
}

// ---------- Functions ----------
int *expandArray(int *array,int &size)
{

    int newSize = size;
    int value;
    int counter = 0;
    int *temp;
    int *newArray = array;

    while (true)
    {
        cout << "enter new value for the array , stop at 0" << endl;
        cout << "counter: " << counter << " size: " << newSize << endl;
        cin >> value;
     
        if (value == 0)
        {
            break;
        }
        newArray[counter] = value;
        counter++;
        if (counter+1 > newSize)
        {
            cout << "Array is full" << endl;
            temp = new int[newSize * 2];
            if (temp == nullptr)
            {
                cout << "Memory allocation failed" << endl;
                return nullptr;
            }

            //we should memset the new array to 0
            //to avoid garbage values
            memset(temp, 0, (newSize * 2) * sizeof(int));

            //copy the old array to the new array
            for (int i = 0; i < newSize; ++i)
            {
                temp[i] = newArray[i];
            }
            delete[] newArray; // delete the old array
            newArray = temp;   // update the pointer
            newSize *= 2; // update new size
        }
    }
    printArray(newArray, newSize);
    size = newSize;
    return newArray;
}
void printArray(int *array, int size)
{
    for (int i = 0; i < size; ++i)
    {
        cout << array[i] << " ";
    }
    cout << endl;
}