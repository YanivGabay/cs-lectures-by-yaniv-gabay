#include <iostream>

using std::cout;
using std::endl;

// Structure definition
struct Data
{
    int **_data;
    int _num_of_lines;
    int *_lines_len;
};

void initializeData(Data &data, int num_of_lines, int lines[][5], int lines_len[]);
void printData(const Data &data);
void removePrefixRows(Data &data);
void init_keep(bool *keep, int num_of_lines);
void check_prefix(const Data &data, bool *keep);
bool isPrefix(int *line1, int len1, int *line2, int len2);
int countNewRows(const Data &data, bool *keep);
void copy_data(Data &data, int **new_data, int *new_lines_len, bool *keep);


int main()
{
    int lines[][5] = {
        {0, 3, 17, -1, -1},   // #0
        {0, 3, 17, 4, 5},     // #1
        {3, 3, 17, 18, -1},   // #2
        {-1, -1, -1, -1, -1}, // #3 (empty)
        {0, -1, -1, -1, -1}   // #4
    };
    int lines_len[] = {3, 5, 4, 0, 1};

    Data data;
    initializeData(data, 5, lines, lines_len);
    cout << "Original Data:" << endl;
    printData(data);

    removePrefixRows(data);
    cout << "Data after removing prefix rows:" << endl;
    printData(data);
    return 0;
}

void initializeData(Data &data, int num_of_lines, int lines[][5], int lines_len[])
{
    data._num_of_lines = num_of_lines;
    data._data = new int *[num_of_lines];
    data._lines_len = new int[num_of_lines];

    for (int i = 0; i < num_of_lines; ++i)
    {
        data._lines_len[i] = lines_len[i];
        if (lines_len[i] == 0)
        {
            data._data[i] = nullptr;
            continue;
        }
        data._data[i] = new int[data._lines_len[i]];
        for (int j = 0; j < data._lines_len[i]; ++j)
        {
            data._data[i][j] = lines[i][j];
        }
    }
}

void printData(const Data &data)
{
    for (int i = 0; i < data._num_of_lines; ++i)
    {
        if (data._data[i] == nullptr)
        {
            cout << "Line #" << i << ": is empty " << endl;
            continue;
        }
        for (int j = 0; j < data._lines_len[i]; ++j)
        {
            // Ignore -1 placeholders
            cout << data._data[i][j] << " ";
        }
        cout << endl;
    }
}

void removePrefixRows(Data &data)
{
    bool *keep = new bool[data._num_of_lines];
    init_keep(keep, data._num_of_lines);

    check_prefix(data, keep);

    int new_num_of_lines = countNewRows(data, keep);


    int **new_data = new int*[new_num_of_lines];
    int *new_lines_len = new int[new_num_of_lines];

    copy_data(data, new_data, new_lines_len, keep);
    data._num_of_lines = new_num_of_lines;
}

void copy_data(Data &data, int **new_data, int *new_lines_len, bool *keep)
{
    int new_index = 0;
    for (int i = 0; i < data._num_of_lines; ++i)
    {
        if (keep[i])
        {
            new_data[new_index] = data._data[i];
            new_lines_len[new_index] = data._lines_len[i];
            new_index++;
        }
       
    }

    delete[] data._data;
    delete[] data._lines_len;
    delete[] keep;

    data._data = new_data;
    data._lines_len = new_lines_len;
   
}

void init_keep(bool *keep, int num_of_lines)
{
    for (int i = 0; i < num_of_lines; ++i)
    {
        keep[i] = true;
    }
}

void check_prefix(const Data &data, bool *keep)
{
     for (int i = 0; i < data._num_of_lines; ++i) {
        for (int j = 0; j < data._num_of_lines; ++j) {
            if (i != j && keep[i] && keep[j] && isPrefix(data._data[i], data._lines_len[i], data._data[j], data._lines_len[j])) {
                keep[i] = false;
                break;
            }
        }
    }
}

bool isPrefix(int *line1, int len1, int *line2, int len2)
{
    if (len1 > len2)
    {
        return false;
    }
    for (int i = 0; i < len1; ++i)
    {
        if (line1[i] != line2[i])
        {
            return false;
        }
    }
    return true;
}

int countNewRows(const Data &data, bool *keep)
{
    int count = 0;
    for (int i = 0; i < data._num_of_lines; ++i)
    {
        if (keep[i])
        {
            ++count;
        }
    }
    return count;
}