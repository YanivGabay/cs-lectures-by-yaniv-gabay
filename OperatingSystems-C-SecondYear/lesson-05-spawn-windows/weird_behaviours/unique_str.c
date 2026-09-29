/*
 * unique_str.c — String uniqueness check (utility)
 *
 * Key concepts: character counting, duplicate detection
 * Compile: gcc -o unique unique_str.c
 * Run:     ./prog
 */
#include <stdio.h>

int main(int argc, char *argv[]) {
    printf("\n");
    printf("========================================\n");
    printf("  String uniqueness check (utility)\n");
    printf("========================================\n\n");
    printf("[unique_str] Received %d argument(s):\n\n", argc);
    printf("  %-8s | %s\n", "Index", "Value");
    printf("  -------- | -----\n");
    for (int i = 0; i < argc; i++) {
        printf("  argv[%d]  | \"%s\"\n", i, argv[i]);
    }
    printf("\n");
    if (argc == 2) {
        printf("  String was passed as a SINGLE argument (correct)\n");
    } else if (argc > 2) {
        printf("  String was SPLIT into %d args (Windows shell quirk!)\n", argc - 1);
    }
    printf("\n========================================\n");
    return 0;
} 