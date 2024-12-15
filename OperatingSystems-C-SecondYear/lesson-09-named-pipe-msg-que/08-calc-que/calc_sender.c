// calc_sender.c

#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>

// Define message types using const int for better type safety and clarity
const int MSG_TYPE_CALC_REQUEST = 1;
const int MSG_TYPE_CALC_RESPONSE = 2;
const int MSG_TYPE_EXIT = 3;

// Structure for calculation requests
struct calc_request {
    char operation;     // '+', '-', '*', '/'
    double operand1;
    double operand2;
};

// Structure for calculation responses
struct calc_response {
    double result;
    char status;        // '0' for success, '1' for error (e.g., division by zero)
};

// Structure for message buffer
struct my_msgbuf {
    long mtype;                     // Message type
    union {
        struct calc_request req;    // Calculation request
        struct calc_response res;   // Calculation response
    } data;
};

int main() {
    key_t key;
    int msgid;
    struct my_msgbuf buf;

    // Generate a unique key using ftok
       key = ftok("/tmp", 65); 
    if (key == -1) {
        perror("ftok");
        exit(EXIT_FAILURE);
    }

    // Access the message queue
    msgid = msgget(key, 0666 | IPC_CREAT);
    if (msgid == -1) {
        perror("msgget");
        exit(EXIT_FAILURE);
    }

    printf("Calculator Sender: Enter operations in the format <op> <num1> <num2>\n");
    printf("Example: + 5 3\n");
    printf("Type 'e' to terminate the receiver.\n");

    while (1) {
        char operation;
        double num1, num2;

        printf("Calculator Sender: ");
        fflush(stdout); // Ensure prompt is printed immediately

        // Read the operation
        int ret = scanf(" %c", &operation);
        if (ret != 1) {
            printf("Invalid input. Please try again.\n");
            // Clear input buffer
            while (getchar() != '\n');
            continue;
        }

        // Check for exit condition
        if (operation == 'e' || operation == 'E') { // 'e' or 'E' to exit
            // Clear the rest of the line
            while (getchar() != '\n');

            // Prepare exit message
            buf.mtype = MSG_TYPE_EXIT;
            memset(&buf.data, 0, sizeof(buf.data)); // Clear data

            // Send exit message with no data
            if (msgsnd(msgid, &buf, 0, 0) == -1) { // Size 0 since no data
                perror("msgsnd");
                exit(EXIT_FAILURE);
            }

            printf("Calculator Sender: Exit message sent. Exiting.\n");
            break;
        }

        // Read operands
        ret = scanf("%lf %lf", &num1, &num2);
        if (ret != 2) {
            printf("Invalid operands. Please try again.\n");
            // Clear input buffer
            while (getchar() != '\n');
            continue;
        }

        // Prepare calculation request
        struct calc_request req;
        req.operation = operation;
        req.operand1 = num1;
        req.operand2 = num2;

        // Prepare message buffer
        buf.mtype = MSG_TYPE_CALC_REQUEST;
        buf.data.req = req;

        // Send the calculation request
        if (msgsnd(msgid, &buf, sizeof(req), 0) == -1) {
            perror("msgsnd");
            exit(EXIT_FAILURE);
        }

        printf("Calculator Sender: Calculation request sent.\n");

        // Wait for the calculation response
        if (msgrcv(msgid, &buf, sizeof(buf.data), MSG_TYPE_CALC_RESPONSE, 0) == -1) {
            perror("msgrcv");
            exit(EXIT_FAILURE);
        }

        // Deserialize the response
        struct calc_response res = buf.data.res;

        // Display the result or error
        if (res.status == '0') {
            printf("Calculator Sender: Result = %.2lf\n", res.result);
        }
        else {
            printf("Calculator Sender: Error in calculation.\n");
        }
    }

    return 0;
}
