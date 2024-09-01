



// to write the same program in c:
//https://en.cppreference.com/w/c/io //IMPORTANT
#include <stdio.h>
//https://en.cppreference.com/w/c/io/fscanf
int main()
{
    // there are MANY variant of scanf and printf
    // scanf() and printf() are the most basic ones
    //basic scanf:
    int number;
    printf("Enter a number: ");
    scanf("%d", &number);
    printf("You entered: %d\n", number);

    //now we will show sscanf which is used to read 
    //from a string
    //its very helpful, when you have a string and you need to
    // extract some data from it
    //example:
    char data[] = "123 456";
    int a, b;
    // (parameters: string, format, variables)
    sscanf(data, "%d %d", &a, &b);
    printf("First number: %d \nSecond number: %d\n", a, b);

    // there is also fprintf which is
    // used to write to a file
    //aswell as fscanf
    //we will show it later

    // another usefull in my opinion is snprintf
    // which instead of printing to std output, it prints to a string
    // which buffer overflow protection
    // example:

    char buffer[10]; // Small buffer to demonstrate overflow protection
    int length = snprintf(buffer, sizeof(buffer), "Hello, %s!", "World");
    printf("Formatted string: %s (Length: %d)\n", buffer, length);

    return 0;
}