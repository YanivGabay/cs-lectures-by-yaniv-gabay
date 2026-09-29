/*
 * child.c — Child — reads stdin, writes stdout (duped from pipes)
 *
 * Key concepts: Reads from redirected stdin, writes to redirected stdout
 * Compile: gcc -o child child.c
 * Run:     ./prog
 */
// child_program.c
#include <stdio.h>
#include <string.h>

int main() {

    printf("\n");
    printf("══════════════════════════════════════\n");
    printf("  Child\n");
    printf("══════════════════════════════════════\n\n");
    char buffer[100];

    // Read message from parent (via redirected stdin ← pipe)
    if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        buffer[strcspn(buffer, "\n")] = '\0';
        // stdout goes back to parent via the other pipe
        printf("[Child] Received from parent: \"%s\"\n", buffer);
    }

    printf("[Child] Sending reply back to parent.\n");

    return 0;
}
