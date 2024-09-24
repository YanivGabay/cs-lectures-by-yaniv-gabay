#include <iostream>
#include <fstream>
#include <cstring>

#include <windows.h> 


using std::cout;
using std::endl;

void mergeFiles(const char *file1, const char *file2, const char *file3);
void write_names(char * names_from_1,char * names_from_2 ,std::ofstream &output);
void clear_line(std::ifstream &file);
void read_single_name(char *line, char *name, int &index);

int main()
{
    mergeFiles("input_1.txt", "input_2.txt", "output.txt");
    cout << "done" << endl;
    return 0;
}

void mergeFiles(const char *file1, const char *file2, const char *file3)
{
    // first create the file streams
    std::ifstream first_input(file1);
    std::ifstream second_input(file2);
    std::ofstream output_file(file3);
    // you can also use the open method
    // std::ifstream file1Stream;
    // file1Stream.open(file1);
    // check if the files are open
    if (!first_input.is_open() || !second_input.is_open() || !output_file.is_open())
    {
        std::cerr << "Error opening files" << std::endl;
        return;
    }

    // now all three are open
    // we can start reading from the first two files
    // important to notice, the numbers are ordered in the files
    // and the names are lexicographically ordered
    // also the format is always:
    // number name1 name2 ...
    // each row has at least one number and one name

    // we will read the first number from each file
    int number1, number2;
    first_input >> number1;
    second_input >> number2;
    cout << "read the first number from the first file: " << number1 << endl;
    cout << "read the first number from the second file: " << number2 << endl;
    while (!first_input.eof() || !second_input.eof())
    {

        if (number1 < number2)
        // means we need to continue to next number in the first file
        {
            // we need to skip the rest of the line
            // from the number 1 file
            cout << "number1 is less than number2" << endl;
            clear_line(first_input);
            first_input >> number1;
            cout << "read the next number from the first file: " << number1 << endl;
        }
        else if (number1 > number2)
        {
            // we need to skip the rest of the line
            // from the number 2 file
            clear_line(second_input);
            second_input >> number2;
            cout << "read the next number from the second file: " << number2 << endl;
        }
        else if (number1 == number2)
        {
            // if they are equal
            // we need to write the number
            // and than check for equal names
            cout << "number1 is equal to number2" << endl;
            output_file << number1 << " ";
            char names_1[1024], names_2[1024];
            // get the line from both files
            first_input.getline(names_1, 1024);
            second_input.getline(names_2, 1024);
            cout << "after getline content of names_1: " << names_1 << endl;
            cout << "after getline content of names_2: " << names_2 << endl;
            write_names(names_1,names_2, output_file);
            output_file << std::endl;
            // and read the next number from both files
         
            first_input >> number1;
            second_input >> number2;
        }
    }
}
void write_names(char * names_from_1,char * names_from_2 ,std::ofstream &output)
{


    char name_file1[100], name_file2[100];
    //we have recived the total line from each file
    // now we just need to get one name from each file at a time
    // and compare them
    // if they are equal we write them to the output file
    // if they are not equal we need to read the next name from the file depend on lex order
    //so the state of the line we read is: " name1 name2 name3 ..."
    //pay attention we have a leading space before the first name
    //so we need to skip it
    int index_file_1 = 1;
    int index_file_2 = 1;

    read_single_name(names_from_1, name_file1, index_file_1);
    read_single_name(names_from_2, name_file2, index_file_2);
    
    cout << "read the first name from the first file: " << name_file1 << endl;
    cout << "read the first name from the second file: " << name_file2 << endl;
    while (name_file1[0] != '\0' && name_file2[0] != '\0')
    {
        if (strcmp(name_file1, name_file2) < 0)
        {//means the name_file1 is lexicographically smaller
            //which means we need to read another name
            //from the first file
            cout << "strcmp is less than 0" << "for strings: " << name_file1 << " and " << name_file2 << endl;
            //so we need to read the next name, but it has space before it
            read_single_name(names_from_1, name_file1, index_file_1);
          
            cout << "read the next name from the first file: " << name_file1 << endl;
            
           
        }
        
        else if (strcmp(name_file1, name_file2) > 0)
        {
            cout << "strcmp is greater than 0" << "for strings: " << name_file1 << " and " << name_file2 << endl;
           
            read_single_name(names_from_2, name_file2, index_file_2);
            cout << "read the next name from the second file: " << name_file2 << endl;
            
        }
        else if (strcmp(name_file1, name_file2) == 0)
        {
            cout << "strcmp is equal to 0" << "for strings: " << name_file1 << " and " << name_file2 << endl;
            output << name_file1 << " ";
            //and read the next name from both files
           read_single_name(names_from_1, name_file1, index_file_1);
              read_single_name(names_from_2, name_file2, index_file_2);

        }
        Sleep(2000);
    }
}
void read_single_name(char *line, char *name, int &index)
{
    //skip the leading space
    while(line[index] == ' ')
    {
        index++;
    }

    int i = 0;
    while(line[index] != ' ' && line[index] != '\0')
    {
        name[i] = line[index];
        i++;
        index++;
    }
    name[i] = '\0';
    cout << "read the name: " << name << endl;
   
}
    
void clear_line(std::ifstream &file)
{
    cout << "clearing the line" << endl;
    char buffer[100];
    file.getline(buffer, 100);
    cout << "cleared the line" << endl;
}