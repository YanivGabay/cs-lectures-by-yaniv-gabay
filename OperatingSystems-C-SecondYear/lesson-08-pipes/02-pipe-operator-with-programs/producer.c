/*
 * producer.c — Producer — writes data to stdout (used with shell pipe)
 *
 * Key concepts: printf to stdout, paired with consumer via pipe operator
 * Compile: gcc -o producer producer.c
 * Run:     ./prog
 */


/// the producer program will write data to stdout

// we need to run both compiled programs (consumer and producer) : ./producer | ./consumer

#include <stdio.h>
#include <unistd.h>
int main(int argc, char const *argv[])
{

    printf("\n");
    printf("══════════════════════════════════════\n");
    printf("  Producer\n");
    printf("══════════════════════════════════════\n\n");
    fprintf(stderr, "[Producer PID %d] Writing data to stdout...\n", getpid());
    fprintf(stderr, "[Producer] Usage: ./producer | ./consumer\n");
    printf("Yaniv\n");
    fprintf(stderr, "[Producer PID %d] Wrote \"Yaniv\" to stdout. Exiting.\n", getpid());
    return 0;
}
