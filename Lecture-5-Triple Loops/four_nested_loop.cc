///////------//lecture 5 - triple loops - 4 nested loops example//------///////

//we will start with a 3 nested loop example, and then we will add another loop
// and see how it works.

//include section
#include <iostream>
//main section
int main() {

    //variables section
    // i own 3 pants, 3 suits, and 2 shoes
    // i want to see all the possible combinations of pants, suits and shoes
    // i can wear.
    int maxPants = 3; // Red, Green, Blue
    int maxSuit = 2;  // Small, Large
    int maxShoes = 2; // Circle, Square
   // int maxWatch = 2; // Smooth, Rough

    for (int color = 1; color <= maxPants; color++) {
        for (int size = 1; size <= maxSuit; size++) {
            for (int shape = 1; shape <= maxShoes; shape++) {
                /*
                //after we understand the triple, i suddenly
                remembered that i have a 2 more watches to add to
                the comination, so i added another loop.
                */
                /*
                for (int texture = 1; texture <= maxWatch; texture++) {
                   
                              << ", Watch " << texture << std::endl;
                }
                */

                std::cout << "Combination: Pants " << color
                              << ", Suit " << size
                              << ", Shoes " << shape << std::endl;
            }
        }
    }

    return 0;
}
