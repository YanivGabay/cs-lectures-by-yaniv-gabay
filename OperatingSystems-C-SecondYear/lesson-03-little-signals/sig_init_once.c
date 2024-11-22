#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

// Custom signal handler for SIGINT
void custom_handler(int sig) {
    printf("\nCaught SIGINT. Switching to default behavior...\n");
    signal(SIGINT, SIG_DFL); // Restore default handler
}

int main() {
    // Set the custom signal handler
    signal(SIGINT, custom_handler);

    printf("Press Ctrl+C to trigger SIGINT. Default behavior will be restored after the first signal.\n");
    while (1) {
        // Infinite loop
    }

    return 0;
}
