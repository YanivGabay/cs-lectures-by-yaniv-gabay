///////------//lecture 5 - triple loops - 5 nested loop example//------///////

/* 
    we go over a given code, with 5 nested loops, and try to understand it.
*/

//include section
#include <iostream>
//main section
int main() {
    const int dimSize = 3; // Size for each dimension
    // bad naming of variables
    //we will go over this, and decide for ourselves what is the best naming convention.

   
    for (int i = 0; i < dimSize; ++i) {
       
        for (int j = 0; j < dimSize; ++j) {
            
            for (int k = 0; k < dimSize; ++k) {
               
                for (int l = 0; l < dimSize; ++l) {
                   
                    for (int m = 0; m < dimSize; ++m) {
                        std::cout << "* ";
                    }
                    std::cout << "  "; // Space between 5th dimension elements
                }
                std::cout << std::endl; // New line for 4th dimension
            }
            std::cout << std::endl; // New line for 3rd dimension
        }
        std::cout << std::endl; // New line for 2nd dimension
    }

    return 0;
}
