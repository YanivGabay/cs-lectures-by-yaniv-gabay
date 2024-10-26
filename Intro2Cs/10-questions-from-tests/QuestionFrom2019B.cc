#include <iostream>
using namespace std;

const int ROWS = 4;
const int COLS = 8;

void process2DArray(int arr[][COLS]);
void findModeAndFrequency(int row[], int &mode, int &frequency);
bool isModeUnique(int arr[][COLS], int currentRow, int mode, int frequency);
int main() {
    int arr[ROWS][COLS] = {
        {17, 17, 5, 4, 3, 2, 1, 0},
        {9, 9, 9, 9, 17, 17, 17, 0},
        {9, 9, 9, 1, 2, 3, 4, 0},
        {6, 4, 5, 4, 6, 6, 5, 0}
    };

    process2DArray(arr);
    return 0;
}
void findModeAndFrequency(int row[], int &mode, int &frequency) {
    frequency = 0;
    for (int i = 0; i < COLS; ++i) {
        int count = 0;
        for (int j = 0; j < COLS; ++j) {
            if (row[i] == row[j]) {
                count++;
            }
        }
        if (count > frequency) {
            frequency = count;
            mode = row[i];
        }
    }
}

bool isModeUnique(int arr[][COLS], int currentRow, int mode, int frequency) {
    for (int i = 0; i < ROWS; ++i) {
        if (i != currentRow) {
            int count = 0;
            for (int j = 0; j < COLS; ++j) {
                if (arr[i][j] == mode) {
                    count++;
                }
            }
            if (count >= frequency) {
                return false;
            }
        }
    }
    return true;
}

void process2DArray(int arr[][COLS]) {
    for (int i = 0; i < ROWS; ++i) {
        int mode, frequency;
        findModeAndFrequency(arr[i], mode, frequency);
        if (isModeUnique(arr, i, mode, frequency)) {
            cout << "Row " << (i + 1) << ": Mode = " << mode << ", Frequency = " << frequency << endl;
        }
    }
}