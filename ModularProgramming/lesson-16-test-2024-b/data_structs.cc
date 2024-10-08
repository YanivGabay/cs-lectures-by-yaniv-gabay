#include <iostream>
#include <stdio.h>
#include <string.h>

using std::cout;
using std::endl;

// Constant definitions
const int MAX_SIZE = 100;

// Struct definitions
struct Data
{
    int **data;
    int num_of_lines;
    int *line_len;
};

// Function prototypes
void add_line(const Data &curr_data, Data &next_data, const int curr_line);
void copy_pointers(const Data &next_data, int **new_data);
int check_for_values(Data &next_data, int *curr_line, const int max_new_line_size);
void look_for_values(const Data &next_data, int &curr_value, int &actual_size);
void print_data(const Data &data);
void copy_new_line(const Data &curr_data, int **new_data, int old_num_of_lines, int curr_line, int max_new_line_size);

// main section
int main()
{

    // tried to copy the test example
    Data curr_data;
    curr_data.line_len = new int[3];
    curr_data.line_len[0] = 3;
    curr_data.line_len[1] = 2;
    curr_data.line_len[2] = 5;

    curr_data.data = new int *[3];
    curr_data.data[0] = new int[3];
    curr_data.data[0][0] = 5;
    curr_data.data[0][1] = 7;
    curr_data.data[0][2] = 17;

    curr_data.data[1] = new int[2];
    curr_data.data[1][0] = 17;
    curr_data.data[1][1] = 38;

    curr_data.data[2] = new int[5];
    curr_data.data[2][0] = 1;
    curr_data.data[2][1] = 2;
    curr_data.data[2][2] = 5;
    curr_data.data[2][3] = 17;
    curr_data.data[2][4] = 77;

    curr_data.num_of_lines = 3;

    Data next_data;

    next_data.line_len = new int[2];
    next_data.line_len[0] = 3;
    next_data.line_len[1] = 2;

    next_data.data = new int *[2];
    next_data.data[0] = new int[3];
    next_data.data[0][0] = 5;
    next_data.data[0][1] = 7;
    next_data.data[0][2] = 17;

    next_data.data[1] = new int[2];
    next_data.data[1][0] = 17;
    next_data.data[1][1] = 38;

    next_data.num_of_lines = 2;
    // TEST CODE START FROM HERE
    cout << "Before adding a new line:" << endl;
    cout << "Current data:" << endl;
    print_data(curr_data);
    cout << "Next data:" << endl;
    cout << "has: " << next_data.num_of_lines << " num of lines" << endl;
    print_data(next_data);

    int curr_line = 2;
    add_line(curr_data, next_data, curr_line);
    cout << "After adding a new line:" << endl;
    cout << " Next data:" << endl;
    cout << "has: " << next_data.num_of_lines << " num of lines" << endl;
    for (int i = 0; i < next_data.num_of_lines; i++)
    {
        cout << "next_data.line_len[" << i << "] = " << next_data.line_len[i] << endl;
    }
    print_data(next_data);
    return 0;
}

// Function definitions
void add_line(const Data &curr_data, Data &next_data, const int curr_line)
{ // we should add a new line, into next_data struct
    // that new line, should have values from the curr_data.data[curr_line]
    // only if those values are not already in the next_data.data
    // if they are, we should skip them
    // we should also update next_data.num_of_lines and next_data.line_len
    // we should also update next_data.data

    // lets get the basic variables that will help us
    //  important to note that in Data there will only be numbers > 0 , and they
    //  are asending order

    // so we now know, that the new line that suppose to be added, will be
    // maximum size of curr_data.line_len[curr_line] ,
    // for each value we need to check if it is already in the next_data.data
    // if not we add it, if it does, we can change it to 0  and not copy it.

    // check for values, will return the actual size needed for new allocation
    //  also will put 0 instead of values we dont need from curr line
    int max_new_line_size = curr_data.line_len[curr_line];
    int actual_size = check_for_values(next_data, curr_data.data[curr_line], max_new_line_size);
    cout << "actual size: " << actual_size << endl;

    if (actual_size > 0)
    {
        int old_num_of_lines = next_data.num_of_lines;
        int **new_data = new int *[old_num_of_lines + 1]; // Allocate space for one additional line

        copy_pointers(next_data, new_data);

        // Allocate space for the new line
        new_data[old_num_of_lines] = new int[actual_size];

        // Copy the values from curr_data to the new line in new_data
        copy_new_line(curr_data, new_data, old_num_of_lines, curr_line, max_new_line_size);

        // Allocate new line_len array with one additional element
        int *new_line_len = new int[old_num_of_lines + 1];
        // Copy old line lengths
        for (int i = 0; i < old_num_of_lines; i++)
        {
            new_line_len[i] = next_data.line_len[i];
        }
        cout << "finished copying line_len" << endl;

        // Set the line length of the new line
        new_line_len[old_num_of_lines] = actual_size;
        cout << "new cell in line_len: " << new_line_len[old_num_of_lines] << " in cell old_num_of_lines: " << old_num_of_lines << endl;

        // Clean up old data
        delete[] next_data.line_len;
        next_data.line_len = new_line_len;

        delete[] next_data.data;
        next_data.data = new_data;

        // Increment num_of_lines after all allocations and assignments are done
        next_data.num_of_lines = old_num_of_lines + 1;
        cout << " new num of lines: " << next_data.num_of_lines << endl;
    }
    else
    {
        cout << "the line couldnt be added, sorry" << endl;
    }
}
void copy_new_line(const Data &curr_data, int **new_data, int old_num_of_lines, int curr_line, int max_new_line_size)
{
    int new_col_index = 0;
    for (int i = 0; i < max_new_line_size; i++)
    {
        int value_to_copy = curr_data.data[curr_line][i];
        if (value_to_copy != 0)
        { // Skip zeros
            new_data[old_num_of_lines][new_col_index++] = value_to_copy;
            cout << "copying: " << value_to_copy << endl;
            cout << "new_data[" << old_num_of_lines << "][" << new_col_index - 1 << "] = " << new_data[old_num_of_lines][new_col_index - 1] << endl;
        }
    }
    cout << "finished copying" << endl;
}
void copy_pointers(const Data &next_data, int **new_data)
{
    for (int i = 0; i < next_data.num_of_lines; i++)
    {
        new_data[i] = next_data.data[i];
    }
}

int check_for_values(Data &next_data, int *curr_line, const int max_new_line_size)
{

    cout << "max_new_line_size: " << max_new_line_size << endl;
    int actual_size = 0;
    for (int i = 0; i < max_new_line_size; i++)
    {
        cout << "curr_line[i]: " << curr_line[i] << endl;
        look_for_values(next_data, curr_line[i], actual_size);
    }
    cout << "returning actual_size: " << actual_size << endl;
    return actual_size;
}
void look_for_values(const Data &next_data, int &curr_value, int &actual_size)
{
    //we were also told each line is in ascending order
    //so if we reach a value that is bigger than curr_value, we can stop
    // so we next_data.data[i][j] > curr_value // we can stop the inner loop
    for (int i = 0; i < next_data.num_of_lines; i++)
    {
        for (int j = 0; j < next_data.line_len[i] || next_data.data[i][j] > curr_value; j++)
        {
            if (next_data.data[i][j] == curr_value)
            {
                cout << "found: " << curr_value << endl;
                curr_value = 0;
                return;
            }
        }
    }
    // if we finished the loop and we didnt find the value
    actual_size++;
}

void print_data(const Data &data)
{
    cout << "num of lines: " << data.num_of_lines << endl;

    for (int i = 0; i < data.num_of_lines; i++)
    {
        cout << "line: " << i << " has: " << data.line_len[i] << " values" << endl;
        for (int j = 0; j < data.line_len[i]; j++)
        {
            std::cout << data.data[i][j] << " ";
        }
        std::cout << std::endl;
    }
}