/*
 * 00c-printf-width.c — Width, alignment and padding: printing tables
 *
 * Key concepts: %5d (right-aligned), %-10s (left-aligned), %05d (zero padding), %8.2f
 * Compile: gcc -o width 00c-printf-width.c
 * Run:     ./width
 *
 * The number between % and the letter is the MINIMUM width of the field.
 */

#include <stdio.h>

int main(void)
{
    printf("[%d]    no width\n", 42);
    printf("[%5d] width 5, right-aligned\n", 42);
    printf("[%-5d] width 5, left-aligned\n", 42);
    printf("[%05d] width 5, zero-padded\n", 42);
    printf("[%8.2f] width 8, 2 decimals\n", 3.14159);
    printf("[%-8s] string, left-aligned\n", "C");
    printf("[%8s] string, right-aligned\n", "C");

    // A small table: same widths on every row keep the columns straight
    printf("\n%-10s %6s %8s\n", "Name", "Grade", "Average");
    printf("%-10s %6d %8.1f\n", "Dana", 95, 91.25);
    printf("%-10s %6d %8.1f\n", "Avi", 78, 80.5);
    printf("%-10s %6d %8.1f\n", "Noa", 100, 97.75);

    return 0;
}
