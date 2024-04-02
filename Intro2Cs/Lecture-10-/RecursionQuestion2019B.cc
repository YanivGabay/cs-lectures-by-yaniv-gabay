#include <iostream>
using std::cout;
using std::endl;
using std::cin;

///// 2019
//// second term
/// question 3
/// 3.ג
//// recursive question
// to find a all the numbers which has
// a equal length, negative sequence
// from left to right
//////    1  2  1
/////        ^
//////  in this example two starts a negative spiral
// from left and from right/ both sides must hae equal
// length

const int SIZE = 10;
const int MOVE_LEFT = -1;
const int MOVE_RIGHT = 1;
const int GOOD_SEQ = 1;
const int BAD_SEQ = 0;
int checkTerm(int arr[]);


int checkTermRecursion(int arr[], int start);

int find_seq(int arr[], int start, int move);

int main() {
    
    int arr[SIZE] = {1,2,-1,0,9,8,7,17,16,15};
    int value = checkTerm(arr);
    cout<<value;
    return 0;
}

int checkTerm(int arr[]) {
  
  return checkTermRecursion(arr, 1);
}
int checkTermRecursion(int arr[], int start) {
  
    if(start >= SIZE-1)
    return 0;

    int left_length = find_seq(arr,start,MOVE_LEFT);
    int right_sequence = find_seq(arr,start,MOVE_RIGHT);
    
    int valid_sequence = (left_length >0 &&
        left_length == right_sequence) ? GOOD_SEQ : BAD_SEQ;

    return valid_sequence + checkTermRecursion(arr,start+1);   
}
int find_seq(int arr[], int start, int move) {
    
    if(start+move < 0 || start+move >= SIZE ||
    arr[start+move] > arr[start])
    return 0; // not valid

    return 1+find_seq(arr,start+move,move);
}