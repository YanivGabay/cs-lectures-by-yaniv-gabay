#include <iostream>

using std::cout;
using std::endl;

bool binarySearch(const int arr[], const int size, const int target);
bool binarySearchRecursion(int arr[], int left, int right, int target);
int main()
{
    // binary search
    int array[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int target = 3;
    bool result = binarySearchRecursion(array, 0, sizeof(array) / sizeof(int), target);
    cout << "The result is: " << result << endl;

    return 0;
}

bool binarySearchRecursion(int arr[], int left, int right, int target)
{
    if (left > right)
    {
        return false;
    }

    int mid = (left + right) / 2;
    if (arr[mid] == target)
    {
        cout << "found the target" << endl;
        return true;
    }
    else if (arr[mid] > target)
    {
        return binarySearchRecursion(arr, left, mid - 1, target);
    }
    else
    {
        return binarySearchRecursion(arr, mid + 1, right, target);
    }
    return false;
}

bool binarySearch(const int arr[], const int size, const int target)
{
    int left = 0;
    int right = size - 1;

    while (left <= right)
    {
        int mid = right + left / 2;

        if (arr[mid] == target)
        {
            cout << "found the target" << endl;
            return true;
        }
        else if (arr[mid] > target)
        {
            right = mid - 1;
        }
        else
        {
            left = mid + 1;
        }
    }
    return false;
}
