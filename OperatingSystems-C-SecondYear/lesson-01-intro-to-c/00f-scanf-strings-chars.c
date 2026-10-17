/*
 * 00f-scanf-strings-chars.c — Strings and single characters with scanf, and the leftover newline
 *
 * Key concepts: %s stops at whitespace, no & for arrays, %19s limits the length, " %c" skips the '\n'
 * Compile: gcc -o read_text 00f-scanf-strings-chars.c
 * Run:     ./read_text     (try typing two words: Yaniv Gabay)
 */

#include <stdio.h>

int main(void)
{
    char first[20];
    char initial;

    printf("Your full name: ");
    scanf("%19s", first);   // no &: the array name is already an address; 19 + '\0' = 20
    printf("%%s read only \"%s\" — it stops at the first space\n", first);

    // The rest of the line (" Gabay" + Enter) is still waiting in the input buffer:
    // the next scanf would read 'G' from it instead of waiting for you. Throw it away first.
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;

    printf("Your grade letter: ");
    scanf(" %c", &initial);   // the space before %c skips any spaces/Enter left over — a good habit
    printf("Grade '%c' — its ASCII code is %d\n", initial, initial);

    return 0;
}
