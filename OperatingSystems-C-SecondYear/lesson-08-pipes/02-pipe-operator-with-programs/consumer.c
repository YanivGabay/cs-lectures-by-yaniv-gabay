/*
 * consumer.c — Consumer — reads from stdin (used with shell pipe)
 *
 * Key concepts: scanf from stdin, paired with producer: ./producer | ./consumer
 * Compile: gcc -o consumer consumer.c
 * Run:     ./prog
 */


/// the consumer program will read the output of the producer program using stdin

// we need to run both compiled programs (consumer and producer) : ./producer | ./consumer

#include <stdio.h>
#include <unistd.h>
int main(int argc, char const *argv[])
{

    printf("\n");
    printf("══════════════════════════════════════\n");
    printf("  Consumer\n");
    printf("══════════════════════════════════════\n\n");
    fprintf(stderr, "[Consumer PID %d] Reading from stdin...\n", getpid());
    char buffer[100];
    scanf("%99s", buffer); // at most 99 chars + '\0' — never overflow the buffer
    printf("[Consumer PID %d] Received via pipe: \"%s\"\n", getpid(), buffer);
    return 0;
}
