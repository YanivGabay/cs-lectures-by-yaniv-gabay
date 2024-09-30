

// to write the same program in c:
// https://en.cppreference.com/w/c/io //IMPORTANT
#include <stdio.h>

int main()
{

    int integer;
    float realNumber;
    char character;
    char string[100];

    printf("Enter an integer, a floating-point number, a character, and a string: ");
    scanf("%d %f %c %s", &integer, &realNumber, &character, string); // Note: no & for string
    //we no need &? cus an array name is a pointer to the first element of the array

    printf("You entered integer: %d\n", integer);
    printf("You entered float: %.2f\n", realNumber); // printing float with two decimal places
    printf("You entered character: %c\n", character);
    printf("You entered string: %s\n", string);
    // the scanf stop reading when it reaches a whitespace


    double pi = 3.141592653589793;
    int number = 50;

    // Print pi with varying precision
    printf("Pi with 2 decimal places: %.2f\n", pi);
    printf("Pi with 5 decimal places: %.5f\n", pi);

    // Print numbers with field width of 10 characters
    printf("Number right-aligned (width 10): |%10d|\n", number);
    printf("Number left-aligned  (width 10): |%-10d|\n", number);


    char line[256];
    int day, year;
    char month[20];
    fgetc(stdin); // Consume newline character left behind by previous scanf
    //IMPORTANT IN C, each enter is a new line character
    //what happens if we remove the fgetc?
    printf("Enter date (e.g., January 1, 2020): ");
    fgets(line, sizeof(line), stdin); // Read full line including spaces
    sscanf(line, "%s %d, %d", month, &day, &year); // Parse string into parts
    //pay attention, we have %s %d, %d WE HAVE ',' in the format string
    // so its expected to have a comma in the input
    //else year will be 0
    printf("Month: %s\n", month);
    printf("Day: %d\n", day);
    printf("Year: %d\n", year);
}