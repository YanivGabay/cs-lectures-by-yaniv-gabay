/*
 * 00b-printf-specifiers.c — printf format specifiers: one per type
 *
 * Key concepts: %d %f %c %s %x %e %%, matching each specifier to an argument
 * Compile: gcc -o specifiers 00b-printf-specifiers.c
 * Run:     ./specifiers
 *
 * printf("format", arg1, arg2, ...): every % in the format consumes the next argument,
 * in order. The specifier MUST match the argument's type — the compiler only warns.
 */

#include <stdio.h>

int main(void)
{
    int age = 21;
    double pi = 3.14159265;
    char grade = 'A';
    char name[] = "Dana";        // a string is an array of chars ending with '\0'

    printf("%%d  int:        %d\n", age);
    printf("%%f  double:     %f\n", pi);          // 6 digits after the point by default
    printf("%%.2f 2 digits:  %.2f\n", pi);
    printf("%%e  scientific: %e\n", pi);
    printf("%%c  char:       %c\n", grade);
    printf("%%s  string:     %s\n", name);
    printf("%%x  hex:        %x\n", 255);
    printf("%%%%  a percent sign: 100%%\n");

    // Several values in one call — matched left to right
    printf("%s is %d years old and got %c (pi = %.3f)\n", name, age, grade, pi);

    return 0;
}
