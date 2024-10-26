/*




*/

// include section
#include <iostream>
#include <cstdlib>
#include <ctime> // For time()

// using section
using ::std::cout;
using ::std::endl;

// const section
const int SIZE_OF_BOARD = 3;
// prototypes section
void displayBoard(char board[][SIZE_OF_BOARD]);

void playerMove(char player,char board[][SIZE_OF_BOARD]);

bool isWinner(char player,char board[][SIZE_OF_BOARD]);

bool isDraw(char board[][SIZE_OF_BOARD]);



// main section
int main()
{

    char board[SIZE_OF_BOARD][SIZE_OF_BOARD] = {
        {' ', ' ', ' '},
        {' ', ' ', ' '},
        {' ', ' ', ' '}};

    char currentPlayer = 'X';
    bool gameEnded = false;

    while (!gameEnded)
    {
        displayBoard(board);
        playerMove(currentPlayer,board);
        gameEnded = isWinner(currentPlayer,board);

        if (gameEnded)
        {
            displayBoard(board);
            std::cout << "Player " << currentPlayer << " wins!" << std::endl;
        }
        else if (isDraw(board))
        {
            displayBoard(board);
            std::cout << "It's a draw!" << std::endl;
            gameEnded = true;
        }

        currentPlayer = (currentPlayer == 'X') ? 'O' : 'X'; // Switch player
    }

    return 0;
}

void displayBoard(char board[][SIZE_OF_BOARD])
{
    for (int row = 0; row < SIZE_OF_BOARD; row++)
    {
        for (int col = 0; col < SIZE_OF_BOARD; col++)
        {
            std::cout << board[row][col];
            if (col < SIZE_OF_BOARD-1)
                std::cout << " | ";
        }
        if (row < SIZE_OF_BOARD-1)
            std::cout << "\n---------\n";
    }
    std::cout << std::endl;
}
void playerMove(char player,char board[][SIZE_OF_BOARD])
{
    int row, col;
    do
    {
        std::cout << "Player " << player << ", enter your move (row[1-3] column[1-3]): ";
        std::cin >> row >> col;
        row--; // Adjust for 0-indexed array
        col--;
    } while (row < 0 || row > SIZE_OF_BOARD-1 || col < 0 || col > SIZE_OF_BOARD-1 || board[row][col] != ' ');

    board[row][col] = player;
}
bool isWinner(char player,char board[][SIZE_OF_BOARD])
{
    // Check rows and columns
    for (int i = 0; i < SIZE_OF_BOARD; i++)
    {
        if ((board[i][0] == player && board[i][1] == player && board[i][2] == player) ||
            (board[0][i] == player && board[1][i] == player && board[2][i] == player))
        {
            return true;
        }
    }
    // Check diagonals
    if ((board[0][0] == player && board[1][1] == player && board[2][2] == player) ||
        (board[0][2] == player && board[1][1] == player && board[2][0] == player))
    {
        return true;
    }
    return false;
}

bool isDraw(char board[][SIZE_OF_BOARD])
{
    for (int row = 0; row < SIZE_OF_BOARD; row++)
    {
        for (int col = 0; col < SIZE_OF_BOARD; col++)
        {
            if (board[row][col] == ' ')
            {
                return false;
            }
        }
    }
    return true;
}
