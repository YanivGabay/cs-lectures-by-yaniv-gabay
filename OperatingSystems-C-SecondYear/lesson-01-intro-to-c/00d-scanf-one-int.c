/*
 * 00d-scanf-one-int.c — Reading one number with scanf (instead of cin >>)
 *
 * Key concepts: scanf("%d", &n), why the & is needed, scanf's return value
 * Compile: gcc -o read_int 00d-scanf-one-int.c
 * Run:     ./read_int      (type a number and press Enter — then try typing "abc")
 *
 * C++:  std::cin >> n;
 * C:    scanf("%d", &n);   — scanf needs the ADDRESS of n (&n) so it can write into it.
 *       Forgetting the & is the classic first C bug: scanf writes to a random address.
 */

#include <stdio.h>

int main(void)
{
    int n = 0;

    printf("Enter a whole number: ");
    int matched = scanf("%d", &n);   // returns how many values it managed to read

    if (matched == 1)
        printf("You entered %d, twice that is %d\n", n, n * 2);
    else
        printf("That was not a number (scanf returned %d)\n", matched);

    return 0;
}
