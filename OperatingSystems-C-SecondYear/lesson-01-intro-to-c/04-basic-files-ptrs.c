
/*

 OLD EXAMPLE OF HOW WE WOULD USE FILES IN LEGACY CPP:

int main() {
    // Writing to a file
    std::ofstream outFile("example.txt");
    if (outFile.is_open()) {
        outFile << "Hello, this is a test file.\n";
        outFile << "Writing another line.";
        outFile.close(); // Explicit close is optional due to RAII
    } else {
        std::cerr << "Unable to open file for writing.\n";
    }

    // Reading from a file
    std::ifstream inFile("example.txt");
    std::string line;
    if (inFile.is_open()) {
        while (getline(inFile, line)) {
            std::cout << line << '\n';
        }
        inFile.close(); // Explicit close is optional due to RAII
    } else {
        std::cerr << "Unable to open file for reading.\n";
    }

    return 0;
}
*/

//in C it will look like this:
#include <stdio.h>
#include <stdlib.h>
int main() {
    // Writing to a file
    FILE *outFile = fopen("example.txt", "w");
    if (outFile != NULL) {
        fprintf(outFile, "Hello, this is a test file.\n");
        fprintf(outFile, "Writing another line.");
        fclose(outFile); // Must explicitly close the file
    } else {
        //perror == cerr
        perror("Unable to open file for writing");
    }

    // Reading from a file
    FILE *inFile = fopen("example.txt", "r");
    char buffer[255];
    if (inFile != NULL) {
        printf("\tFile contents:\n");
        while (fgets(buffer, sizeof(buffer), inFile)) {
            printf("\t%s", buffer);
        }
        fclose(inFile); // Must explicitly close the file
    } else {
        perror("Unable to open file for reading");
    }

    //// we probably want to use the same file for both reading and writing
    //// and not to open and close it twice
    //// so we can use the "r+" mode
    FILE *file = fopen("example.txt", "r+");
    if (file != NULL) {
        //we just opened a file, so the pointer is at the beginning
        //so if the file is not empty, we will overwrite it
        //if we want to append, we can use fseek
        //https://en.cppreference.com/w/c/io/fseek
        fseek(file, 0, SEEK_END); // seek to end of file // we can also use SEEK_SET and SEEK_CUR
        //    (file, 0 means we move 0 bytes from the end of the file , seek-end is the end of the file)
        // Write to the file
        fprintf(file, "\nAppending a new line.\n");
        //what happends here? the ptr suppose to be at the end of the file
        //so if we dont reset it to the beginning, we will not read anything
        // Read from the file
        printf("\n\n\tFile contents using r+ :\n");
        while (fgets(buffer, sizeof(buffer), file)) {
            printf("\t%s", buffer);
        }

        //so now we will use the same code but with fseek before
        fseek(file, 0, SEEK_SET); // seek to the beginning of the file
        //there is also rewind(file) but im not sure yoram approves it
        
        printf("\n\n\tFile contents using r+ and seek_set :\n");
        while (fgets(buffer, sizeof(buffer), file)) {
            printf("\t%s", buffer);
        }

        fclose(file); // Must explicitly close the file
    } else {
        perror("Unable to open file for reading and writing");
    }

    

    return 0;
}


