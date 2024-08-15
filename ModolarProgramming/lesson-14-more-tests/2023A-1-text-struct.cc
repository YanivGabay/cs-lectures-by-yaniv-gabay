/********************************************************************
 * Course: Modolar Programming
 * Lecture: 13 - tests
 * File: 2023A-1-text-struct.cc
 *
 *
 * Author: Yaniv Gabay
 * Date: 2024-07-26
 *
 * Overview:
 *
 *
 * Notes:
 * Any additional notes related to this file.
 *******************************************************************/

// ---------- Include Section ----------
#include <iostream>
// Add more #include directives as needed

// ---------- Using Section ----------
using std::cerr;
using std::cin;
using std::cout;
using std::endl;
// ---------- Constants ----------
const int MAX_SIZE = 100;

// ---------- Structs ----------
struct Text
{
    char **_text;
    int _num_of_lines;
    int *_line_length;
};
// ---------- Function Prototypes ----------
void modify_text(struct Text *text, const int row, const int col);

char *get_values(char *line, const int start, const int end);
void print_text(const struct Text *const text);

void fix_my_row(struct Text *text, const int row, const int col);
// ---------- Main Function ----------
int main()
{

    // lets create a simple text struct
    // values {a,b,c},{?,!},{1,2,3,4,5}
    // lines_length {3,2,5}
    // num_of_lines 3
    char **text = new char *[3];
    text[0] = new char[3]{'a', 'b', 'c'};
    text[1] = new char[2]{'?', '!'};
    text[2] = new char[5]{'1', '2', '3', '4', '5'};
    int *line_length = new int[3]{3, 2, 5};
    
    struct Text my_text = {text, 3, line_length};
    // there is no /0 at the end of the lines
    // there is no empty rows/values

    // the function shouldnt return anything (not stated)
    //  given a row number and col, move all values from that col and right,
    //  to the next row, and push all other rows.
    //  so given 0,2
    //  the text should be:
    //  {a,b},{c},{?,!},{1,2,3,4,5}
    //  lines_length {2,1,2,5}
    //  num_of_lines 4
    std::cout << "before:" << std::endl;
    print_text(&my_text);
    modify_text(&my_text, 0, 2);
    cout << "after:" << endl;
    print_text(&my_text);

    return 0;
}

// ---------- Functions ----------
void modify_text(struct Text *text, const int row, const int col)
{
    if (text == nullptr || text->_line_length == nullptr || text->_text == nullptr)
    {
        cerr << "invalid input" << endl;
        return;
    }
    // we can assume the row and col are valid and we dont need to check them.

    // so first step, is copy the values from the position till end
    int new_line_length = text->_line_length[row] - col;
    char *values_to_move = get_values(text->_text[row], col, text->_line_length[row]);
    // now we need to delete the values from the original row
    fix_my_row(text, row, col);
    text->_line_length[row] = col; // new length
    ///////////
    
    // now we need to push all the rows down
    // first we need to create a new array of pointers
    char **new_text = new char *[text->_num_of_lines + 1];
    int *new_line_lengths = new int[text->_num_of_lines + 1];
    int new_num_of_lines = text->_num_of_lines + 1;

    int target_row = 0;
    // copy the values from the original array
    for (int i = 0; i < new_num_of_lines; i++, target_row++)
    {
        if (i == row + 1)
        {
            new_text[i] = values_to_move;
            new_line_lengths[i] = new_line_length;
            target_row--;
        }
        else
        {
            new_text[i] = text->_text[target_row];
            new_line_lengths[i] = text->_line_length[target_row];
        }
    }
    // we are copying pointers, so we dont need to delete all the values
    delete[] text->_line_length;
    //maybe i forgot to delete something?
    

    // replace the old arrays with the new arrays
    text->_text = new_text;
    text->_line_length = new_line_lengths;
    text->_num_of_lines = new_num_of_lines;
}

void fix_my_row(struct Text *text, const int row, const int col)
{
    // new size for the line is the col we cut
    char *new_row = new char[col];
    // move values into this array:
    for (int i = 0; i < col; i++)
    {
        new_row[i] = text->_text[row][i];
    }

    // delete the old row
    delete[] text->_text[row];
    // replace the row with the new row
    text->_text[row] = new_row;
}

char *get_values(char *line, const int start, const int end)
{
    char *values = new char[end - start];
    //check alloc std::nothrow
    for (int i = start; i < end; i++)
    {
        values[i - start] = line[i];
    }
    return values;
}
              
void print_text(const struct Text *const text)
{
    for (int i = 0; i < text->_num_of_lines; i++)
    {
        for (int j = 0; j < text->_line_length[i]; j++)
        {
            cout << text->_text[i][j];
        }
        cout << endl;
    }
}