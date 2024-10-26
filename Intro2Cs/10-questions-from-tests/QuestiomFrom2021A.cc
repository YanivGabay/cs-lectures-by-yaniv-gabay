#include <iostream>
using std::cout;
using std::endl;
using std::cin;

//basic question
//but what is the running time.
//outer loop does N-1 actions
//inner loop start from i+1 to N = n-i-1
//we again get a sequence of 1+2+3+...+n-1 = n(n-1)/2
//which is equal to n^2 SIDRA HESHBONIT
//imagine we start with the first index, than check n-1 cells
//we havnt found , so we start with the second cell, and check n-2 cells
//and so on until we start with the n-1 cell and check 1 cell


//WE CAN IMPROVE THIS CODE SPECIFICLY FOR YORAM TEST
//WHAT YOU WILL CHANGE??????????


// next year with Eran London you will learn how to do this with O(n) time
//using hash table
bool find_sum(int arr[], int N, int sum, int &index1, int &index2) ;
const int N = 10;


int main() {
    
    int arr[N] = {12, 15, -2, 5, 0, 77, 31, 3, -2, 4};
    int sum = 35;
    int index1, index2;

    bool result = find_sum(arr, N, sum, index1, index2);

    if (result) {
        cout << "Sum found at indices " << index1 << " and " << index2 << endl;
    } else {
        cout << "No two numbers add up to " << sum << endl;
    }

    return 0;
}
bool find_sum(int arr[], int N, int sum, int &index1, int &index2) {
    for (int i = 0; i < N - 1; ++i) {
        for (int j = i + 1; j < N; ++j) {
            if (arr[i] + arr[j] == sum) {
                index1 = i;
                index2 = j;
                return true;
            }
        }
    }
    return false;
}
