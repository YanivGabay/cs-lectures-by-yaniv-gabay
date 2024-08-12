/********************************************************************
 * Course: Modolar Programming
 * Lecture: Lecture 1 - BackTracking
 * File: QueensProblem.cc
 * 
 * Author: Yaniv Gabay
 * Date: 2024-04-CURRENT_DAY
 * 
 * Overview:
 * Queen can move in all directions in chess, and how many steps she wants
 * in this example, we want to on a board of 4X4 , to put 4 queens, where
 * they wont be able to (eat) each other.
 * this is a classic exampleof backtracking, and very famous
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
const int N = 4; // for board size
// ---------- Function Prototypes ----------
void printSolution(int board[N][N]);
bool isSafe(int board[N][N], int row, int col) ;
bool solveNQUtil(int board[N][N], int col) ;
bool solveNQ();
// ---------- Main Function ----------
int main() {
    solveNQ();
    return 0;
}

// ---------- Functions ----------
void printSolution(int board[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++)
            std::cout << " " << board[i][j] << " ";
        std::cout << std::endl;
    }
}
// Function to check if a queen can be placed on board[row][col]
bool isSafe(int board[N][N], int row, int col) {
    int i, j;

    // Check this row on left side
    for (i = 0; i < col; i++)
        if (board[row][i])
            return false;

    // Check upper diagonal on left side
    for (i = row, j = col; i >= 0 && j >= 0; i--, j--)
        if (board[i][j])
            return false;

    // Check lower diagonal on left side
    for (i = row, j = col; j >= 0 && i < N; i++, j--)
        if (board[i][j])
            return false;

    return true;
}
bool solveNQUtil(int board[N][N], int col) {
    // If all queens are placed then return true
    if (col >= N)
        return true;

    // Consider this column and try placing this queen in all rows one by one
    for (int i = 0; i < N; i++) {
        // Check if the queen can be placed on board[i][col]
        if (isSafe(board, i, col)) {
            // Place this queen in board[i][col]
            board[i][col] = 1;

            // Recur to place rest of the queens
            if (solveNQUtil(board, col + 1))
                return true;

            // If placing queen in board[i][col] doesn't lead to a solution, then remove queen from board[i][col]
            board[i][col] = 0; // BACKTRACK
        }
    }

    // If the queen cannot be placed in any row in this column col, then return false
    return false;
}
// This function solves the N Queens problem using Backtracking.
bool solveNQ() {
    int board[N][N] = { { 0, 0, 0, 0 },
                        { 0, 0, 0, 0 },
                        { 0, 0, 0, 0 },
                        { 0, 0, 0, 0 } };

    if (!solveNQUtil(board, 0)) {
        std::cout << "Solution does not exist";
        return false;
    }

    printSolution(board);
    return true;
}