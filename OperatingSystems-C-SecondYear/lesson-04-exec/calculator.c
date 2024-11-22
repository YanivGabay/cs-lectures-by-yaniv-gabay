#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc != 4) { // Expect exactly 3 arguments: num1, operator, num2
        fprintf(stderr, "Usage: %s <num1> <operator> <num2>\n", argv[0]);
        return 1;
    }

    // Parse arguments
    int num1 = atoi(argv[1]);
    char operator = argv[2][0];
    int num2 = atoi(argv[3]);
    int result = 0;

    // Perform calculation based on the operator
    switch (operator) {
        case '+':
            result = num1 + num2;
            break;
        case '-':
            result = num1 - num2;
            break;
        case '*':
            result = num1 * num2;
            break;
        case '/':
            if (num2 == 0) {
                fprintf(stderr, "Error: Division by zero\n");
                return 1;
            }
            result = num1 / num2;
            break;
        default:
            fprintf(stderr, "Error: Invalid operator '%c'\n", operator);
            return 1;
    }

    // Print the result
    printf("Result: %d %c %d = %d\n", num1, operator, num2, result);
    return 0;
}
