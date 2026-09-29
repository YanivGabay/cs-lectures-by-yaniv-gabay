/*
 * 02-more-basic-input-output.c — More I/O — multiple data types and format specifiers
 *
 * Key concepts: scanf, printf, data types
 * Compile: gcc -o prog 02-more-basic-input-output.c
 * Run:     ./prog
 */




// to write the same program in c:
//https://en.cppreference.com/w/c/io //IMPORTANT
#include <stdio.h>
//https://en.cppreference.com/w/c/io/fscanf
int main()
{
    // there are MANY variant of scanf and printf
    // scanf() and printf() are the most basic ones
    //basic scanf:
    printf("\n");
    printf("========================================\n");
    printf("  C I/O Variants: scanf, sscanf, snprintf\n");
    printf("========================================\n\n");

    // --- Section 1: Basic scanf ---
    printf("--- Section 1: Basic scanf (reading from keyboard) ---\n\n");
    int number;
    printf("  Enter a number: ");
    fflush(stdout);
    scanf("%d", &number);
    printf("  You entered: %d\n\n", number);

    // --- Section 2: sscanf (parsing from a string) ---
    printf("--- Section 2: sscanf (reading from a string) ---\n\n");
    //its very helpful, when you have a string and you need to
    // extract some data from it
    char data[] = "123 456";
    int a, b;
    // (parameters: string, format, variables)
    sscanf(data, "%d %d", &a, &b);
    printf("  Input string: \"%s\"\n", data);
    printf("  sscanf(data, \"%%d %%d\", &a, &b)\n");
    printf("  Result: a = %d, b = %d\n\n", a, b);

    // --- Section 3: snprintf (safe formatted printing to a string) ---
    printf("--- Section 3: snprintf (print to string, not screen) ---\n\n");
    // another usefull in my opinion is snprintf
    // which instead of printing to std output, it prints to a string
    // which buffer overflow protection

    char buffer[10]; // Small buffer to demonstrate overflow protection
    int length = snprintf(buffer, sizeof(buffer), "Hello, %s!", "World");
    printf("  Buffer size: %zu bytes\n", sizeof(buffer));
    printf("  snprintf(buffer, %zu, \"Hello, %%s!\", \"World\")\n", sizeof(buffer));
    printf("  Result:  \"%s\"\n", buffer);
    printf("  Wanted length: %d  (truncated because buffer is only %zu bytes)\n\n", length, sizeof(buffer));

    printf("========================================\n");
    printf("  End of demo\n");
    printf("========================================\n");

    return 0;
}