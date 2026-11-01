/*
 * args-flags.c — Options like real Linux commands: ./greet -n 3 -u Dana
 *
 * Key concepts: walking argv, strcmp(), options that take a value, a usage message, exit codes
 * Compile: gcc -o greet args-flags.c
 * Run:     ./greet -n 3 -u Dana        (then try: ./greet -n)
 *
 * Commands such as `ls -l` or `gcc -o prog file.c` read their options exactly like this:
 * look at each argv[i]; if it is an option, maybe consume the NEXT argument as its value.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>   // strcmp
#include <ctype.h>    // toupper

static void usage(const char *prog)
{
    fprintf(stderr, "Usage: %s [-n times] [-u] name\n", prog);
    fprintf(stderr, "  -n times   say hello this many times (default 1)\n");
    fprintf(stderr, "  -u         shout: print the name in UPPERCASE\n");
}

int main(int argc, char *argv[])
{
    int times = 1;
    int upper = 0;
    const char *name = NULL;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-n") == 0) {
            if (i + 1 >= argc) {                  // "-n" must be followed by a value
                fprintf(stderr, "Error: -n needs a number\n");
                usage(argv[0]);
                return EXIT_FAILURE;
            }
            times = atoi(argv[++i]);              // ++i: the value is the NEXT argument
        } else if (strcmp(argv[i], "-u") == 0) {
            upper = 1;
        } else if (argv[i][0] == '-') {
            fprintf(stderr, "Error: unknown option %s\n", argv[i]);
            usage(argv[0]);
            return EXIT_FAILURE;
        } else {
            name = argv[i];                       // anything else is the name
        }
    }

    if (name == NULL) {
        usage(argv[0]);
        return EXIT_FAILURE;
    }

    for (int t = 0; t < times; t++) {
        printf("Hello, ");
        for (const char *c = name; *c; c++)
            putchar(upper ? toupper((unsigned char)*c) : *c);
        printf("!\n");
    }
    return EXIT_SUCCESS;
}
