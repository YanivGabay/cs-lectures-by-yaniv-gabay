#include <iostream>



// Constant definitions
const int MAX_SIZE = 100;

// Struct definitions
struct Data {
    int ** data;
    int num_of_lines;
    int * line_len;
};

// Function prototypes



//main section
int main() {
   
    //this main wont work
    // need to add some data to the structs
    Data curr_data;
    Data next_data;
    int curr_line = 2;
    add_line(curr_data,next_data,curr_line);
    return 0;
}

// Function definitions
void add_line(const Data &curr_data, Data &next_data,const int curr_line)
{
    //we should add a new line, into next_data struct
    //that new line, should have values from the curr_data.data[curr_line]
    //only if those values are not already in the next_data.data
    //if they are, we should skip them
    //we should also update next_data.num_of_lines and next_data.line_len
    //we should also update next_data.data

    //lets get the basic variables that will help us
    // important to note that in Data there will only be numbers > 0 , and they 
    // are asending order

    // so we now know, that the new line that suppose to be added, will be
    // maximum size of curr_data.line_len[curr_line] , 
    //for each value we need to check if it is already in the next_data.data
    //if not we add it, if it does, we can change it to 0  and not copy it.


    int max_new_line_size = curr_data.line_len[curr_line];
    //check for values, will return the actual size needed for new allocation
    // also will put 0 instead of values we dont need from curr line
    int actual_size = check_for_values(next_data,curr_data.data[curr_line],max_new_line_size);

    //now we are ready to allocate the new line
    // of course we need to first allocate a new int** data in the new_data
    // also we need to modify num_oflines and *line_len with the new len;
    if (actual_size > 0)
    {
        int ** new_data = new int* [next_data.num_of_lines+1];//additional line
        //we can copy the previos pointers instead of just copying the values
        
        copy_pointers(next_data,new_data);
        
        new_data[next_data.num_of_lines+1] = new int[actual_size];
        //now we can copy the values
        for (int i = 0; i < actual_size; i++)
        {
            if(curr_data.data[curr_line][i] != 0)//we dont want to trasfter the 0s
            new_data[next_data.num_of_lines+1][i] = curr_data.data[curr_line][i];
        }

        //now just update the two other values
        next_data.num_of_lines++;
        //need to allocate new line_len
        int * new_line_len = new int[next_data.num_of_lines];//num of lines is already updated
        //copy old values
        for (int i = 0; i < next_data.num_of_lines-1; i++)
        {
            new_line_len[i] = next_data.line_len[i];
        }
        new_line_len[next_data.num_of_lines] = actual_size;
        delete [] next_data.line_len;
        next_data.line_len = new_line_len;

        //now we need to say goodbye to the old data
        delete [] next_data.data;
        next_data.data = new_data;

    }


}
void copy_pointers(const Data& next_data, int** new_data)
{
    for (int i = 0; i < next_data.num_of_lines; i++)
    {
        new_data[i] = next_data.data[i];
    }
}

int check_for_values(const Data &next_data, int * curr_line, const int max_new_line_size)
{
   

    int actual_size = 0;
    for (int i = 0; i < max_new_line_size; i++)
    {
        look_for_values(next_data,curr_line,max_new_line_size,actual_size);
        
    }
    return actual_size;
}
void look_for_values(const Data& next_data, int* curr_line,const int max_new_line_size,int& actual_size)
{
    for (int next_data_row; next_data_row < next_data.num_of_lines; next_data_row++)
        {
            for (int next_data_col; next_data_col < next_data.line_len[next_data_row]; next_data_col++)
            {
                if (next_data.data[next_data_row][next_data_col] == curr_line[i])
                {
                   //if we found the value in the next_data
                   // we should NOT add him
                   //so we put a 0
                   curr_line[i] = 0;
                }
                else 
                {   //we havnt found him
                    //so actual size++
                     actual_size++;
                    
                }
            }
        }
}