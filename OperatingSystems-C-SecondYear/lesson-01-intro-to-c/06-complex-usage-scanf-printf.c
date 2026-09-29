/*
 * 06-complex-usage-scanf-printf.c — Advanced scanf/printf formatting — width, precision, flags
 *
 * Key concepts: format specifiers, width modifiers, precision
 * Compile: gcc -o prog 06-complex-usage-scanf-printf.c
 * Run:     ./prog
 */


// to write the same program in c:
// https://en.cppreference.com/w/c/io //IMPORTANT
#include <stdio.h>

int main()
{

    printf("\n");
    printf("══════════════════════════════════════\n");
    printf("  Advanced scanf/printf formatting\n");
    printf("══════════════════════════════════════\n\n");

    printf("\n");
    // --- Section 1: Multi-type scanf ---
    printf("--- Section 1: Reading multiple types at once ---\n\n");
    int integer;
    float realNumber;
    char character;
    char string[100];

    printf("  Enter: integer float char string (e.g., 42 3.14 A hello): ");
    fflush(stdout);
    scanf("%d %f %c %s", &integer, &realNumber, &character, string);

    printf("\n  %-12s | %s\n", "Type", "Value");
    printf("  ------------ | -----\n");
    printf("  %%d (int)     | %d\n", integer);
    printf("  %%f (float)   | %.2f\n", realNumber);
    printf("  %%c (char)    | '%c'\n", character);
    printf("  %%s (string)  | \"%s\"\n\n", string);

    // --- Section 2: Precision and width ---
    printf("--- Section 2: Precision and width formatting ---\n\n");
    double pi = 3.141592653589793;
    int number = 50;

    printf("  Pi with different precision:\n");
    printf("    %%.2f  -> %.2f\n", pi);
    printf("    %%.5f  -> %.5f\n", pi);
    printf("    %%.10f -> %.10f\n\n", pi);

    printf("  Width formatting (| shows boundaries):\n");
    printf("    %%10d  (right-aligned) -> |%10d|\n", number);
    printf("    %%-10d (left-aligned)  -> |%-10d|\n\n", number);

    // --- Section 3: Parsing a date string ---
    printf("--- Section 3: Parsing a formatted string ---\n\n");
    char line[256];
    int day, year;
    char month[20];
    fgetc(stdin); // Consume leftover newline from previous scanf

    printf("  Enter a date (e.g., January 1, 2020): ");
    fflush(stdout);
    fgets(line, sizeof(line), stdin);
    sscanf(line, "%s %d, %d", month, &day, &year);

    printf("\n  Parsed result:\n");
    printf("    Month: %s\n", month);
    printf("    Day:   %d\n", day);
    printf("    Year:  %d\n", year);
    if (year == 0) printf("    NOTE: Year is 0 — did you forget the comma?\n");

    printf("\n========================================\n");
    printf("  End of demo\n");
}