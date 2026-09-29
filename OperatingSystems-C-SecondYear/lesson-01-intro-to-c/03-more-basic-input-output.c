/*
 * 03-more-basic-input-output.c — Safe string input with fgets instead of gets
 *
 * Key concepts: fgets, buffer overflow protection, gets vs fgets
 * Compile: gcc -o prog 03-more-basic-input-output.c
 * Run:     ./prog
 */




// to write the same program in c:
//https://en.cppreference.com/w/c/io //IMPORTANT
#include <stdio.h>

int main()
{

    printf("\n");
    printf("========================================\n");
    printf("  String Input & printf Formatting\n");
    printf("========================================\n\n");

    // --- Section 1: gets vs fgets ---
    printf("--- Section 1: gets() vs fgets() ---\n\n");

    // 1. gets = not recommended, not safe
    // it keeps reading chars regardless of buffer size until \n or \0
    char buffer[100];
    printf("  [gets] Enter a string (WARNING: gets has no size limit!): ");
    fflush(stdout);
    gets(buffer);  // Unsafe usage
    printf("  [gets] You entered: \"%s\"\n", buffer);
    printf("  NOTE: gets() can overflow the buffer — NEVER use in real code!\n\n");

    // fgets is the safe alternative
    char buffer_2[100];
    printf("  [fgets] Enter a string (safe — limited to buffer size): ");
    fflush(stdout);
    fgets(buffer_2, sizeof(buffer_2), stdin);  // Safe usage
    printf("  [fgets] You entered: \"%s\"", buffer_2);
    printf("  NOTE: fgets() includes the \\n in the result\n\n");

    // --- Section 2: Reading single characters ---
    printf("--- Section 2: Reading single characters ---\n\n");

    char ch;
    printf("  Enter a single character: ");
    fflush(stdout);
    scanf("%c", &ch);
    printf("  scanf(\"%%c\") read: '%c' (ASCII %d)\n", ch, ch);
    // Swallow the leftover newline from pressing Enter
    scanf("%c", &ch);

    char ch_2;
    printf("  Enter another character (using getchar): ");
    fflush(stdout);
    ch_2 = getchar();
    printf("  getchar() read: '%c' (ASCII %d)\n\n", ch_2, ch_2);

    // --- Section 3: printf string formatting ---
    printf("--- Section 3: printf string formatting ---\n\n");

    const char* s = "Hello";

    printf("  String: \"%s\"\n\n", s);
    printf("  %-15s | %s\n", "Format", "Output");
    printf("  --------------- | ------\n");
    printf("  %%s              | [%s]\n", s);
    printf("  %%10s  (right)   | [%10s]\n", s);
    printf("  %%-10s (left)    | [%-10s]\n", s);
    printf("  %%.3s  (3 chars) | [%.3s]\n", s);
    printf("  %%10.3s          | [%10.3s]\n", s);
    printf("  %%*s  (* = 10)   | [%*s]\n\n", 10, s);

    printf("========================================\n");
    printf("  End of demo\n");
    printf("========================================\n");




}