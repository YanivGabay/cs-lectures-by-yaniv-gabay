///////------//lecture 5 - triple loops - intro to arrray//------///////

/* 
    we will just show basic capabilities of arrays, and how to use them.
    basic fill and print
*/


//include section
#include <iostream>

//main section
int main() {
    // variables section
    int numbers[20]; // this is an array of 40 integers, named bible.
    //we created an array, with 40 values, first value is at index 0, last value is at index 39.
    //we can access the values in the array, by using the index of the value.
    //and do whatever we want with it (but still within the boundaries of type of the variables).
    
    numbers[0] = 17;
    std::cin >> numbers[19];
    numbers[19]++;
    std::cout << "numbers[0]: " << numbers[0] << std::endl;
    std::cout << "numbers[19]: " <<numbers[19] << std::endl;
    // we are NOT allowed to:
    /*
    cin >> bible;
    cout << bible;
    bible = 0;
    etc...
        
    */
    //very good friends with loops?
    //fill the array with 0's
    for (int index = 0; index < 20; index++)
    {
        numbers[index] = 0;
    }
    //print the array
    for (int index = 0; index < 20; index++)
    {
        std::cout<< "["<<numbers[index] << "]"<< ' ';
        if(index%10 == 9) // bonus quiz: what is this condition?
        {
            std::cout << std::endl;
        }
    }
    std::cout << std::endl;
    //fill some values again, so we see diff output
    numbers[0] = 17;
    numbers[13] = 13;
    numbers[2] = 6;
    numbers[19]++;

    //just more information about arrays when printing them.
    //print the array
    for (int index = 0; index < 20; index++)
    {
        std::cout<< index <<"." << "["<<numbers[index] << "]"<< ' ';
        if(index%10 == 9) // bonus quiz: what is this condition?
        {
            std::cout << std::endl;
        }
    }
    


    return 0;
}
