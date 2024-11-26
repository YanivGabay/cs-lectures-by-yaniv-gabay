
#include <process.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    char program_name[100];
    char input_string[100];


    ////// this example is to recreate wierd C windows run time behaviours
    // using spawn
    //// if we pass a string like "aa bb cc" as an argument to the program
    // without the quotes, the spawn function, and the c windows run time, will separate the string
    // into 3 different argv arguments so: aa, bb, cc
    // but we we pass the string with quotes, the spawn function will pass the string as a single argument
    // try for yourself, run the program first and enter the following:
    // program name: ./unique_str
    // input string: aa bb cc      - without qutoes

    // than run it again and enter the following:
    // program name: ./unique_str
    // input string: "aa bb cc"    - with quotes

    // Get the name of the program to run
    printf("Enter the program name to run (e.g., unique_str): ");
    scanf("%99s", program_name);

    // Get the input string
    printf("Enter the string to pass as a single argument (e.g., \"aa bb cc\"): ");
    getchar(); // Consume the newline left in the buffer
    fgets(input_string, sizeof(input_string), stdin);

    // Remove newline character if present
    size_t len = strlen(input_string);
    if (len > 0 && input_string[len - 1] == '\n') {
        input_string[len - 1] = '\0';
    }
    printf("received from user: %s\n", input_string);
    // Arguments for spawn
    char *args[] = {program_name, input_string, NULL};

    // Spawn the program with _P_WAIT so the parent waits for the child to complete
    int status = spawnv(_P_WAIT, program_name, args);
    if (status == -1) {
        perror("spawnv failed");
        return 1;
    } else {
        printf("Program finished with status: %d\n", status);
    }

    return 0;
}