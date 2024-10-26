
///////------//lecture 6 - bubblesort visualisation on arrays//------///////

/*
  bubble sort
  with visualisation
  you dont need to understand this code
  its only for us to print
*/

//include section
#include <iostream>
#include <cstdlib>
using ::std::cout;
using ::std::endl;
//const section
const int CLASS_SIZE = 5;
const int MAX_GRADE = 100;
const int MIN_GRADE = 0;

// Function to print the array
void printArray(int arr[], int size, int round = -1, int index = -1) {
    for (int i = 0; i < size; i++) {
        if (round != -1 && (i == index || i == index + 1)) { // Highlight the elements being compared
            cout << "[" << arr[i] << "] ";
        } else {
            cout << arr[i] << " ";
        }
    }
    cout << endl;
}

int main() {
    int students_id[CLASS_SIZE];
    // Reading grades ensuring they are within the valid range
    for (int stud = 0; stud < CLASS_SIZE; stud++) {
        do {
            cout << "Enter grade for student " << stud + 1 << ": ";
            std::cin >> students_id[stud];
        } while (students_id[stud] < MIN_GRADE || students_id[stud] > MAX_GRADE);
    }

    cout << "Initial Grades: ";
    printArray(students_id, CLASS_SIZE);

    // Bubble Sort with visual output
    for (int round = 0; round < CLASS_SIZE - 1; round++) {
        cout << "After Round " << round + 1 << ": " << endl;
        for (int stud = 0; stud < CLASS_SIZE - round - 1; stud++) {
            if (students_id[stud] > students_id[stud + 1]) {
                // Swap elements
                int temp = students_id[stud];
                students_id[stud] = students_id[stud + 1];
                students_id[stud + 1] = temp;

                // Print the array highlighting the swapped elements
                printArray(students_id, CLASS_SIZE, round, stud);
            }
        }
    }

    cout << "Sorted Grades: ";
    printArray(students_id, CLASS_SIZE);

    return 0;
}
