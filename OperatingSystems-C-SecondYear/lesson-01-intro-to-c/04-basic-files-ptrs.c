/*
 * 04-basic-files-ptrs.c — File I/O — fopen, fclose, fprintf, fscanf
 *
 * Key concepts: fopen, fclose, fprintf, fscanf, file modes
 * Compile: gcc -o prog 04-basic-files-ptrs.c
 * Run:     ./prog
 */

/*

 OLD EXAMPLE OF HOW WE WOULD USE FILES IN LEGACY CPP:

int main() {

    printf("\n");
    printf("══════════════════════════════════════\n");
    printf("  File I/O\n");
    printf("══════════════════════════════════════\n\n");
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
    printf("\n");
    // --- Section 1: Write to a file ---
    printf("--- Section 1: Writing to a file (mode \"w\") ---\n\n");
    FILE *outFile = fopen("example.txt", "w");
    if (outFile != NULL) {
        fprintf(outFile, "Hello, this is a test file.\n");
        fprintf(outFile, "Writing another line.");
        fclose(outFile);
        printf("  Wrote 2 lines to example.txt\n\n");
    } else {
        perror("  ERROR: Unable to open file for writing");
    }

    // --- Section 2: Read from a file ---
    printf("--- Section 2: Reading from a file (mode \"r\") ---\n\n");
    FILE *inFile = fopen("example.txt", "r");
    char buffer[255];
    if (inFile != NULL) {
        printf("  Contents of example.txt:\n");
        while (fgets(buffer, sizeof(buffer), inFile)) {
            printf("    > %s", buffer);
        }
        printf("\n\n");
        fclose(inFile);
    } else {
        perror("  ERROR: Unable to open file for reading");
    }

    // --- Section 3: Read + Write with fseek ---
    printf("--- Section 3: Read+Write mode (\"r+\") with fseek ---\n\n");
    FILE *file = fopen("example.txt", "r+");
    if (file != NULL) {
        // Seek to end, then append
        fseek(file, 0, SEEK_END);
        fprintf(file, "\nAppending a new line.\n");
        printf("  Appended a line using fseek(SEEK_END)\n\n");

        // Try reading without seeking back — nothing will print
        printf("  Reading WITHOUT seeking back to start:\n");
        int count = 0;
        while (fgets(buffer, sizeof(buffer), file)) {
            count++;
            printf("    > %s", buffer);
        }
        if (count == 0) printf("    (nothing — file pointer is at the end!)\n");
        printf("\n");

        // Now seek to beginning and read
        fseek(file, 0, SEEK_SET);
        printf("  Reading AFTER fseek(SEEK_SET) — back to start:\n");
        while (fgets(buffer, sizeof(buffer), file)) {
            printf("    > %s", buffer);
        }
        printf("\n");

        fclose(file);
    } else {
        perror("  ERROR: Unable to open file");
    }

    return 0;
}


