#include <iostream>


using std::cout;
using std::endl;


void printArray(int array[],int size);
void bubbleSort(int arr[],int size);
void innerBubbleSort(int arr[],int size,int index);
void swap(int &a,int &b);

int main()
{
    int array[] = {64,32,42,51,12,22,11,99};
    cout << "Unsorted array: ";
    printArray(array,8);
    bubbleSort(array,8);
    cout << "Sorted array: ";
    printArray(array,8);


    return 0;
}
void printArray(int array[],int size) 
{
    for(int i = 0; i < size; i++)
    {
        cout << array[i] << " ";
    }
    cout << endl;
}

void bubbleSort(int arr[],int size) // == outside for loop
{
    if(size ==1)
    return;

    //one time to the inner loop
    innerBubbleSort(arr,size,0);

    bubbleSort(arr,size-1);
}

void innerBubbleSort(int arr[],int size,int index)
{
    if(index == size-1)
    return; 

    if(arr[index] > arr[index+1])
    {
        swap(arr[index],arr[index+1]);
    }

    innerBubbleSort(arr,size,index+1);


}

void swap(int &a,int &b)
{
    int temp = a;
    a = b;
    b = temp;
}