/*
 * args-copy.c — Your own `cp`: copy the file named in argv[1] to the name in argv[2]
 *
 * Key concepts: two files open at once, modes "r" and "w", fgetc/fputc, checking every call
 * Compile: gcc -o mycp args-copy.c
 * Run:     ./mycp notes.txt copy.txt     (then look at copy.txt in the Files panel)
 *
 * Mode "w" creates the destination — or EMPTIES it if it already exists.
 */
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    if (argc != 3) {
        fprintf(stderr, "Usage: %s source destination\n", argv[0]);
        return EXIT_FAILURE;
    }

    FILE *src = fopen(argv[1], "r");
    if (src == NULL) {
        perror(argv[1]);
        return EXIT_FAILURE;
    }
    FILE *dst = fopen(argv[2], "w");
    if (dst == NULL) {
        perror(argv[2]);
        fclose(src);                  // do not leak the file we DID open
        return EXIT_FAILURE;
    }

    long bytes = 0;
    int c;
    while ((c = fgetc(src)) != EOF) {
        fputc(c, dst);
        bytes++;
    }

    fclose(src);
    if (fclose(dst) != 0) {           // the data is really written at close: check it too
        perror(argv[2]);
        return EXIT_FAILURE;
    }
    printf("Copied %ld bytes: %s -> %s\n", bytes, argv[1], argv[2]);
    return EXIT_SUCCESS;
}
