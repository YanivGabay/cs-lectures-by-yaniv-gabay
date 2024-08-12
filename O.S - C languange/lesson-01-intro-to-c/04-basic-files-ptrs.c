
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
        while (fgets(buffer, sizeof(buffer), inFile)) {
            printf("%s", buffer);
        }
        fclose(inFile); // Must explicitly close the file
    } else {
        perror("Unable to open file for reading");
    }

    return 0;
}


