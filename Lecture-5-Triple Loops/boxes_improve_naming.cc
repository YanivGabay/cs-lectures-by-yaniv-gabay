///////------//lecture 5 - triple loops - boxes example//------///////

/* the same code from boxes.cc, but with better syntax
    and better variable names.
    this suppose to show us, how much better it is
    to read code, when the variables are named properly.
    its a bit unfair cause we already know what the code does,
    but still its much more readbly.
*/

// its reccomended to change the values of length,width,and height
// to see how the box changes.


// include section
#include <iostream>

// main section
int main()
{
    // variables section
    int length = 3; // Length of the box
    int width = 3;  // Width of the box
    int height = 3; // Height of the box

    // Outer loop for the num boxes
    for (int num_of_boxes = 0; num_of_boxes < height; num_of_boxes++)
    {
        // Middle loop for the length
        for (int length_of_box = 0; length_of_box < length; length_of_box++)
        {
            // Inner loop for the width
            for (int width_of_box = 0; width_of_box < width; width_of_box++)
            {
                // Decide when to print the asterisk
                if (num_of_boxes == 0 || num_of_boxes == height - 1 ||
                    length_of_box == 0 || length_of_box == length - 1 ||
                    width_of_box == 0 || width_of_box == width - 1)
                {
                    std::cout << "* ";
                }
                else
                {
                    std::cout << "  "; // Print spaces for the inner part of the box
                }
            }
            std::cout << std::endl; // New line at the end of each width layer
        }
        std::cout << std::endl; // Separate the layers of the box
    }

    return 0;
}
