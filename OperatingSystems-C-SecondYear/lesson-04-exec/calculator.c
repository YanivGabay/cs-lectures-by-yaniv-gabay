/*
 * calculator.c — Calculator child program (used by calculator_exec.c)
 *
 * Key concepts: This is the child program that exec launches
 * Compile: gcc -o calculator calculator.c
 * Run:     ./prog
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>    // for getpid()

int main(int argc, char *argv[]) {

    printf("\n");
    printf("══════════════════════════════════════\n");
    printf("  Calculator child program (used by calculator_exec.c)\n");
    printf("══════════════════════════════════════\n\n");
    printf("\n");
    printf("[Calculator PID %d] Launched by parent via exec().\n", getpid());
    printf("[Calculator] Received %d arguments (expected 3: num op num)\n\n", argc - 1);
    if (argc != 4) {
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

    printf("[Calculator] %d %c %d = %d\n\n", num1, operator, num2, result);
    return 0;
}
