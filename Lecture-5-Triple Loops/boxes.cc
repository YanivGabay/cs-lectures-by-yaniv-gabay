///////------//lecture 5 - triple loops - boxes example//------///////

/* in this example, we will use triple nested loop 
    to print a box of asterisks, with the given length, width and height.
    it will also have a big if statement to decide when to print the asterisk,
    which is fine, and neccessary in this case.
*/


//include section
#include <iostream>

//main section
int main() {
    // variables section
    int length = 3; // Length of the box
    int width = 3;  // Width of the box
    int height = 3; // Height of the box

    // Outer loop for the num boxes
    for (int i = 0; i < height; i++) {
        // Middle loop for the length
        for (int j = 0; j < length; j++) {
            // Inner loop for the width
            for (int k = 0; k < width; k++) {
                // Decide when to print the asterisk
                if (i == 0 || i == height - 1 || j == 0 || j == length - 1 || k == 0 || k == width - 1) {
                    std::cout << "* ";
                } else {
                    std::cout << "  "; // Print spaces for the inner part of the box
                }
            }
            std::cout << std::endl; // New line at the end of each width layer
        }
        std::cout << std::endl; // Separate the layers of the box
    }

    return 0;
}
