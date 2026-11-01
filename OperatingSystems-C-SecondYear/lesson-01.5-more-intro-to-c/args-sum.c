/*
 * args-sum.c — Arguments are strings: turning "42" into the number 42
 *
 * Key concepts: argv[i] is a char*, atoi() vs strtol() with error checking, EXIT_FAILURE
 * Compile: gcc -o sum args-sum.c
 * Run:     ./sum 5 7 30        (then try: ./sum 5 seven 3)
 *
 * "5" + "7" is not 12 — the shell hands every argument to main() as TEXT.
 * atoi("seven") quietly returns 0. strtol() tells you where it stopped reading,
 * so you can refuse input that is not a number.
 */
#include <stdio.h>
#include <stdlib.h>   // atoi, strtol, EXIT_SUCCESS, EXIT_FAILURE

int main(int argc, char *argv[])
{
    if (argc < 2) {
        fprintf(stderr, "Usage: %s number [number ...]\n", argv[0]);
        return EXIT_FAILURE;
    }

    long sum = 0;
    for (int i = 1; i < argc; i++) {
        char *end;
        long value = strtol(argv[i], &end, 10);   // base 10; end = first char NOT used

        printf("argv[%d] = \"%s\"   atoi -> %d   ", i, argv[i], atoi(argv[i]));
        if (end == argv[i] || *end != '\0') {      // nothing read, or junk after the digits
            printf("strtol -> not a number!\n");
            fprintf(stderr, "Error: \"%s\" is not a whole number\n", argv[i]);
            return EXIT_FAILURE;
        }
        printf("strtol -> %ld\n", value);
        sum += value;
    }

    printf("\nSum of %d numbers = %ld\n", argc - 1, sum);
    return EXIT_SUCCESS;
}
