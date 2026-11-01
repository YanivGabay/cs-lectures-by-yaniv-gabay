/*
 * Challenge (Lesson 01.5): check argc before you touch argv
 *
 * This greeter assumes it got a name. The site runs it with NO arguments,
 * so argv[1] is NULL — and strlen(NULL) crashes the program (SIGSEGV).
 *
 * Your task: if argc is not 2, print a usage line to stderr that contains the
 * word "Usage" (for example: Usage: ./greet name) and return EXIT_FAILURE (1).
 * Done when the program ends with "Exited 1" and the usage line — not a crash.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s name\n", argv[0]);
        return EXIT_FAILURE;
    }

    printf("Hello, %s! Your name has %zu letters.\n", argv[1], strlen(argv[1]));
    return EXIT_SUCCESS;
}
