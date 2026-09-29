/*
 * 01-basic-input-output.c — Basic I/O — scanf/printf vs C++ cin/cout
 *
 * Key concepts: scanf, printf, format specifiers
 * Compile: gcc -o prog 01-basic-input-output.c
 * Run:     ./prog
 */


//differences  between c and cpp
/*

#include <iostream>

int main() {

    printf("\n");
    printf("══════════════════════════════════════\n");
    printf("  Basic I/O\n");
    printf("══════════════════════════════════════\n\n");
    int number;
    std::cout << "Enter a number: ";
    std::cin >> number;
    std::cout << "You entered: " << number << std::endl;
    return 0;
}


*/

// to write the same program in c:
//https://en.cppreference.com/w/c/io //IMPORTANT
#include <stdio.h>
//https://en.cppreference.com/w/c/io/fscanf

void print_test(char* input,char* output,char* prase_type)
{
    printf("  Input:  \"%s\"\n", input);
    printf("  Result: \"%s\"  (%s)\n\n", output, prase_type);
}


int main()
{
    printf("\n");
    // --- Section 1: Interactive input ---
    printf("--- Section 1: Reading input with scanf ---\n\n");
    int number;
    printf("  Enter a number: ");
    fflush(stdout);
    scanf("%d", &number);
    // scanf() reads formatted input from the standard input (stdin)
    // sscanf() reads formatted input from a string
    // there is also fscanf() function in c
    // check out more in the link
    printf("  You entered: %d\n\n", number);

   //https://en.cppreference.com/w/c/io/fscanf
    /* conversion specifiers
       %d: a decimal integer.
       %f: a floating-point value
       %c: a character
       [set]: a string of characters that match the set
            - for example %9[0-9] is a string of at most 9 decimal digits
            - google scansets in c for more info
       %s: a string of characters that aren't whitespace
       %9s: a string of at most 9 non-whitespace characters
       %2d: two-digit integer - for example 99
       %*d: an integer which isn't stored anywhere
       ' ': all consecutive whitespace
       %3[0-9]: a string of at most 3 decimal digits
       %i: a integer (can be octa, hexa or dec)
    */

    // --- Section 2: Parsing with sscanf ---
    printf("--- Section 2: Parsing strings with sscanf ---\n\n");
    printf("  %-12s | %-20s | %-10s | %s\n", "Format", "Input", "Result", "Notes");
    printf("  ------------ | -------------------- | ---------- | -----\n");

    char input1[] = "42";
    int num;
    sscanf(input1, "%d", &num);
    printf("  %-12s | %-20s | %-10d | integer\n", "%d", input1, num);

    char input2[] = "3.14";
    float fnum;
    sscanf(input2, "%f", &fnum);
    printf("  %-12s | %-20s | %-10.2f | float\n", "%f", input2, fnum);

    char input4[] = "99";
    int twoDigitNum;
    sscanf(input4, "%2d", &twoDigitNum);
    printf("  %-12s | %-20s | %-10d | 2-digit only\n", "%2d", input4, twoDigitNum);

    char input5[] = "1.23E4";
    float sciNum;
    sscanf(input5, "%f", &sciNum);
    printf("  %-12s | %-20s | %-10.0f | scientific\n", "%f (sci)", input5, sciNum);

    printf("\n");

    // --- Section 3: String parsing with %%s and width ---
    printf("--- Section 3: String parsing with %%s (max 9 chars) ---\n\n");

    char input3[] = "       HelloWorld";
    char input3_a[] = "Hello         World";
    char input3_b[] = "   a b c d e     d   g";
    char str2[10];
    char str2_a[10];
    char str2_b[10];
    sscanf(input3, "%9s", str2);
    printf("  Input: \"       HelloWorld\"   -> %%9s -> \"%s\"\n", str2);
    sscanf(input3_a, "%9s", str2_a);
    printf("  Input: \"Hello         World\" -> %%9s -> \"%s\"  (stops at whitespace)\n", str2_a);
    sscanf(input3_b, "%9s", str2_b);
    printf("  Input: \"   a b c d e...\"     -> %%9s -> \"%s\"  (skips leading spaces)\n", str2_b);

    printf("\n");

    // --- Section 4: Parsing multiple fields at once ---
    printf("--- Section 4: Parsing multiple fields at once ---\n\n");

    int i, j;
    float x, y;
    char str1[10];
    char input[] = "25 54.32E-1 Thompson 56789";
    int ret = sscanf(input, "%d%f%9s%2d%f%*d",
                     &i, &x, str1, &j, &y);

    printf("  Input:  \"%s\"\n", input);
    printf("  Format: \"%%d%%f%%9s%%2d%%f%%*d\"\n\n");
    printf("  Fields converted: %d\n", ret);
    printf("  %%d   -> i    = %d\n", i);
    printf("  %%f   -> x    = %.3f\n", x);
    printf("  %%9s  -> str1 = \"%s\"\n", str1);
    printf("  %%2d  -> j    = %d     (only first 2 digits of 56789)\n", j);
    printf("  %%f   -> y    = %.0f   (remaining digits)\n", y);
    printf("  %%*d  -> skipped       (the * means: read but discard)\n");

    printf("\n");

    // --- Section 5: Scansets [set] ---
    printf("--- Section 5: Scansets — character class matching ---\n\n");

    char input7[] = "123456";
    char str4[4];
    sscanf(input7, "%3[0-9]", str4);
    printf("  %%3[0-9]  : digits only, max 3\n");
    print_test(input7, str4, "first 3 digits");

    //this will stop at first non digit character
    char input6_a[] = "12a3456";
    char str3_a[4];
    sscanf(input6_a, "%3[0-9]", str3_a);
    printf("  %%3[0-9]  : stops at non-digit\n");
    print_test(input6_a, str3_a, "stopped at 'a'");

    char input8[] = "123456X789";
    char str5[10];
    sscanf(input8, "%9[^X]", str5);
    printf("  %%9[^X]   : everything except 'X'\n");
    print_test(input8, str5, "stopped at 'X'");

    char input9[] = "abcd4";
    char str6[10];
    sscanf(input9, "%[a-zA-Z]", str6);
    printf("  %%[a-zA-Z]: alphabetic only\n");
    print_test(input9, str6, "stopped at '4'");

    char input10[] = "Hello World\n abcde";
    char str7[25];
    sscanf(input10, "%[^\n]", str7);
    printf("  %%[^\\n]   : read until newline\n");
    print_test(input10, str7, "stopped at \\n");

    return 0;
}
