#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc != 4) {
        //recived the following arguments:
        printf("argc in calculator: %d\n", argc);
        for (int i = 0; i < argc; i++) {
            printf("argv[%d]: %s\n", i, argv[i]);
        }
        fprintf(stderr, "Usage: %s <num1> <operator> <num2>\n", argv[0]);
        return 1;
    }

    int num1 = atoi(argv[1]);
    char operator = argv[2][0];
    int num2 = atoi(argv[3]);
    int result;

    
    switch (operator) {
        case '+': result = num1 + num2; break;
        case '-': result = num1 - num2; break;
        //the * is a "wildcard" in the shell, so we cant use it as an argument
       // case '*': result = num1 * num2; break;
        case 'X': result = num1 * num2; break;
        case '/': 
            if (num2 == 0) {
                fprintf(stderr, "Error: Division by zero\n");
                return 1;
            }
            result = num1 / num2; 
            break;
        default:
            fprintf(stderr, "Error: Unknown operator '%c'\n", operator);
            return 1;
    }

    printf("(output from calculator) Result: %d %c %d = %d\n", num1, operator, num2, result);
    return 0;
}
