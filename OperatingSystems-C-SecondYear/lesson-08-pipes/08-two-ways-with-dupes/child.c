// child_program.c
#include <stdio.h>
#include <string.h>

int main() {
    char buffer[100];

    // Read message from parent
    if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        // Remove newline character if present
        buffer[strcspn(buffer, "\n")] = '\0';
        printf("Child received: %s\n", buffer);
    }

    // Respond to parent
    printf("Hello from Child Program!\n");

    return 0;
}
