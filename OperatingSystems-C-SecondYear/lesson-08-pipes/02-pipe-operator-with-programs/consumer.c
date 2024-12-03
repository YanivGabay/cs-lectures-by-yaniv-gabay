

/// the consumer program will read the output of the producer program using stdin

// we need to run both compiled programs (consumer and producer) : ./producer | ./consumer

#include <stdio.h>
int main(int argc, char const *argv[])
{
    char buffer[100];
    scanf("%s", buffer);
    printf("Your Name is: %s\n", buffer);
    return 0;
}
