#include <stdio.h>


/*
we need to run this program using the terminal in order to pass it some arguments
but first, lets run it as usual.
*/

int main(int argc, char *argv[]) {
    printf("Number of arguments: %d\n", argc);
    for (int i = 0; i < argc; i++) {
        printf("Argument %d: %s\n", i, argv[i]);
    }
    return 0;
}