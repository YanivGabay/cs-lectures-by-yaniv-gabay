/*
 * 00a-hello-printf.c — Your first C program: printf instead of cout
 *
 * Key concepts: #include <stdio.h>, int main(void), printf, \n and other escape sequences
 * Compile: gcc -o hello 00a-hello-printf.c
 * Run:     ./hello
 *
 * C++:  std::cout << "Hello" << std::endl;
 * C:    printf("Hello\n");
 */

#include <stdio.h>   // printf and scanf live here (C++ used <iostream>)

int main(void)       // (void) = "takes no arguments" — in C, () means "unspecified"
{
    printf("Hello from C!\n");           // \n ends the line — there is no endl
    printf("No newline here... ");
    printf("so this continues the same line.\n");

    printf("Escape sequences:\n");
    printf("  \\n  new line\n");
    printf("  \\t  tab:\t[after a tab]\n");
    printf("  \\\"  a quote: \"quoted\"\n");
    printf("  \\\\  a backslash: \\\n");

    return 0;        // 0 = success; the shell sees it as the exit status ($?)
}
