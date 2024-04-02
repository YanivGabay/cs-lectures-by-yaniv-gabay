/********************************************************************
 * Course: Course Name
 * Lecture: Lecture 1 - BackTracking
 * File: MazeSolving.cc
 * 
 * Author: Yaniv Gabay
 * Date: 2024-04-02
 * 
 * Overview:
 * a more "simpler" backtracking example
 * we will use a maze, and try to find our way out.
 * 
 * IMPORTANT NOTES:
 * our target cell is the right button one of the matrix פינה ימנית תחתונה
 * 1 represent a passable way
 * 0 represent a none passable way
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

const int N = 5; // size of board

// ---------- Function Prototypes ----------
bool solveMazeUtil(int maze[N][N], int x, int y, int sol[N][N]) ;
bool isSafe(int maze[N][N], int x, int y);
void printSolution(int sol[N][N]);
bool solveMaze(int maze[N][N]) ;
// ---------- Main Function ----------
//our actual maze
// 1 reperesent a cell we can go into
// 0 represent a cell we CANT go into
int main() {
int maze[N][N] = {
    {1, 1, 1, 0, 0},
    {0, 0, 1, 0, 0},
    {0, 0, 1, 1, 1},
    {0, 0, 0, 0, 1},
    {1, 1, 1, 1, 1}
};
    solveMaze(maze);
    return 0;
}
// ---------- Functions ----------
void printSolution(int sol[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++)
            cout << sol[i][j] << " ";
        cout << endl;
    }
}

// Function to check if x, y is a valid index for N*N maze
bool isSafe(int maze[N][N], int x, int y) {
    cout << "Checking safety for cell: " << x << ", " << y << endl;
    return (x >= 0 && x < N && y >= 0 && y < N && maze[x][y] == 1);
}
bool solveMaze(int maze[N][N]) {
    int sol[N][N] = { { 0, 0, 0, 0, 0 },
                      { 0, 0, 0, 0, 0 },
                      { 0, 0, 0, 0, 0 },
                      { 0, 0, 0, 0, 0 },
                      { 0, 0, 0, 0, 0 } };

    if (!solveMazeUtil(maze, 0, 0, sol)) {
        cout << "Solution doesn't exist";
        return false;
    }

    printSolution(sol);
    return true;
}
// Function to solve the maze problem using backtracking
bool solveMazeUtil(int maze[N][N], int x, int y, int sol[N][N]) {
    // Check if (x, y) is the destination
    if (x == N - 1 && y == N - 1) {
        sol[x][y] = 1; // Mark the destination as part of the solution path
        return true;
    }

    if (isSafe(maze, x, y) && sol[x][y] == 0) { // Check if the cell is safe and not already part of the solution path
        sol[x][y] = 1; // Mark the current cell as part of the solution path

        // Try moving in each direction
        //right
        if (x + 1 < N && solveMazeUtil(maze, x + 1, y, sol)) return true;
        //down
        if (y + 1 < N && solveMazeUtil(maze, x, y + 1, sol)) return true;
        //left
        if (x - 1 >= 0 && solveMazeUtil(maze, x - 1, y, sol)) return true;
        //up
        if (y - 1 >= 0 && solveMazeUtil(maze, x, y - 1, sol)) return true;

        sol[x][y] = 0; 
        //
        cout << "backtracking we got stuck" << endl;
        // Backtrack: Unmark the current cell as part of the solution path
        return false;
    }

    return false; // Return false if (x, y) is not safe or already part of the solution path
}

