/*
 * 00e-scanf-many.c — Reading several values, of different types, in one scanf
 *
 * Key concepts: %d %lf in one format, %lf for double (printf uses %f), literal characters in the format
 * Compile: gcc -o read_many 00e-scanf-many.c
 * Run:     ./read_many     (try: 7 2.5   then   3,4)
 *
 * Spaces and Enter between numbers are skipped automatically.
 * Any other character in the format (like the comma in "%d,%d") must appear in the input.
 */

#include <stdio.h>

int main(void)
{
    int count;
    double price;

    printf("How many items and the price of one (e.g. 7 2.5): ");
    if (scanf("%d %lf", &count, &price) != 2) {   // %lf reads a double; %f would read a float
        printf("Expected a whole number and a decimal number.\n");
        return 1;
    }
    printf("%d x %.2f = %.2f\n", count, price, count * price);

    int x, y;
    printf("A point as x,y (e.g. 3,4): ");
    if (scanf("%d,%d", &x, &y) == 2)              // the comma must be typed too
        printf("Point (%d, %d)\n", x, y);
    else
        printf("Could not read x,y — did you type the comma?\n");

    return 0;
}
