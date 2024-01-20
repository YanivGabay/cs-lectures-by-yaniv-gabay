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

    for (int pants = 1; pants <= maxPants; pants++) {
        for (int suit = 1; suit <= maxSuit; suit++) {
            for (int shoe = 1; shoe <= maxShoes; shoe++) {
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

                std::cout << "Combination: Pants " << pants
                              << ", Suit " << suit
                              << ", Shoes " << shoe << std::endl;
            }
        }
    }

    return 0;
}
