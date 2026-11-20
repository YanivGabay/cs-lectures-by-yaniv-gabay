/*
 * show_args.c — Print exactly the arguments this program received
 *
 * Key concepts: argc/argv as the child sees them (a helper for 02-args-shell-vs-spawn.c)
 * Compile: gcc -o show_args show_args.c
 * Run:     ./show_args one "two words" "*"
 *
 * Brackets make spaces visible: [two words] is ONE argument.
 */
#include <stdio.h>

int main(int argc, char *argv[])
{
    printf("    show_args got argc=%d:", argc - 1);
    for (int i = 1; i < argc; i++)
        printf(" [%s]", argv[i]);
    printf("\n");
    return 0;
}
