/*
 * args-wc.c — A mini `wc`: count lines, words and characters of every file you name
 *
 * Key concepts: looping over argv, fopen/fgetc/fclose, perror(), keep going after an error
 * Compile: gcc -o mini_wc args-wc.c
 * Run:     ./mini_wc poem.txt notes.txt missing.txt
 *
 * A missing file is reported with perror() but does not stop the others —
 * just like the real `wc`. The exit status tells the shell whether ANY file failed.
 */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>    // isspace

int main(int argc, char *argv[])
{
    if (argc < 2) {
        fprintf(stderr, "Usage: %s file [file ...]\n", argv[0]);
        return EXIT_FAILURE;
    }

    int failed = 0;
    printf("%6s %6s %6s  %s\n", "lines", "words", "chars", "file");

    for (int i = 1; i < argc; i++) {
        FILE *f = fopen(argv[i], "r");
        if (f == NULL) {
            perror(argv[i]);          // e.g. "missing.txt: No such file or directory"
            failed = 1;
            continue;                 // next file
        }

        long lines = 0, words = 0, chars = 0;
        int c, in_word = 0;
        while ((c = fgetc(f)) != EOF) {
            chars++;
            if (c == '\n')
                lines++;
            if (isspace(c))
                in_word = 0;
            else if (!in_word) {      // first letter of a new word
                in_word = 1;
                words++;
            }
        }
        fclose(f);
        printf("%6ld %6ld %6ld  %s\n", lines, words, chars, argv[i]);
    }

    return failed ? EXIT_FAILURE : EXIT_SUCCESS;
}
