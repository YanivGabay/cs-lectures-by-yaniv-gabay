#include <stdio.h>
#include <stdlib.h>


#include <stdio.h>
#include <stdlib.h>

// lets say we are expecting in our program
// to always get a name of a file
// so we can open it and read from it
// we MUST check for argc/argv size
// if we dont, we might get a segfault




int main(int argc, char *argv[]) {
    const int ARGUMENTS_COUNT = 2; 
    if (argc != ARGUMENTS_COUNT) { // if argc is not 2
        perror("Usage: ./a.out <filename>"); // we show the user how to use the program
        return EXIT_FAILURE; // and we return an error
    }
    //here we can continue with the program

    const char* filename = argv[1];

    printf("Filename: %s\n", filename);

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return EXIT_FAILURE;
    }

    // Read from the file
    char buffer[100];
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        printf("%s", buffer);
    }



    return 0;
}