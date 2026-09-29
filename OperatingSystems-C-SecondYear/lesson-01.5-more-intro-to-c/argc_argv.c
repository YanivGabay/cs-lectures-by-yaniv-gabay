/*
 * argc_argv.c — Basic argc/argv — command line arguments
 *
 * Key concepts: argc, argv, main parameters
 * Compile: gcc -o prog argc_argv.c
 * Run:     ./prog arg1 arg2
 */
#include <stdio.h>


/*
we need to run this program using the terminal in order to pass it some arguments
but first, lets run it as usual.
*/

int main(int argc, char *argv[]) {

    printf("\n");
    printf("══════════════════════════════════════\n");
    printf("  Basic argc/argv\n");
    printf("══════════════════════════════════════\n\n");
    printf("\n");
    printf("  %-6s | %s\n", "Index", "Value");
    printf("  ------ | -----\n");
    for (int i = 0; i < argc; i++) {
        printf("  argv[%d] | \"%s\"%s\n", i, argv[i], i == 0 ? "  (program name)" : "");
    }
    printf("\n");
    if (argc == 1) {
        printf("  TIP: Try running with arguments:\n");
        printf("    ./prog hello world 42\n\n");
    }
    return 0;
}