// calc_receiver.c

#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <unistd.h>

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

    // Generate unique key using ftok
      key = ftok("/tmp", 65); 
    if (key == -1) {
        perror("ftok");
        exit(EXIT_FAILURE);
    }

    // Create or access the message queue
    msgid = msgget(key, 0666 | IPC_CREAT);
    if (msgid == -1) {
        perror("msgget");
        exit(EXIT_FAILURE);
    }

    printf("Calculator Receiver: Waiting for calculation requests...\n");

    while (1) {
        // Receive any message type (mtype = 0)
        if (msgrcv(msgid, &buf, sizeof(buf.data), 0, 0) == -1) {
            perror("msgrcv");
            exit(EXIT_FAILURE);
        }

        // Handle different message types
        if (buf.mtype == MSG_TYPE_EXIT) {
            printf("Calculator Receiver: Exit message received. Exiting.\n");
            break;
        }
        else if (buf.mtype == MSG_TYPE_CALC_REQUEST) {
            // Process calculation request
            struct calc_request req = buf.data.req;
            struct calc_response res;
            res.status = '0'; // Assume success

            printf("Calculator Receiver: Received operation '%c' with operands %.2lf and %.2lf\n",
                   req.operation, req.operand1, req.operand2);

            // Perform calculation
            switch (req.operation) {
                case '+':
                    res.result = req.operand1 + req.operand2;
                    break;
                case '-':
                    res.result = req.operand1 - req.operand2;
                    break;
                case '*':
                    res.result = req.operand1 * req.operand2;
                    break;
                case '/':
                    if (req.operand2 != 0) {
                        res.result = req.operand1 / req.operand2;
                    } else {
                        res.status = '1'; // Error: Division by zero
                        res.result = 0.0;
                    }
                    break;
                default:
                    res.status = '1'; // Error: Unknown operation
                    res.result = 0.0;
            }

            // Prepare response message
            struct my_msgbuf res_buf;
            res_buf.mtype = MSG_TYPE_CALC_RESPONSE;
            res_buf.data.res = res;

            // Send response
            if (msgsnd(msgid, &res_buf, sizeof(res), 0) == -1) {
                perror("msgsnd");
                exit(EXIT_FAILURE);
            }

            printf("Calculator Receiver: Sent calculation result.\n");
        }
        else {
            printf("Calculator Receiver: Unknown message type received: %ld\n", buf.mtype);
        }
    }

    // Cleanup: Remove the message queue
    if (msgctl(msgid, IPC_RMID, NULL) == -1) {
        perror("msgctl");
        exit(EXIT_FAILURE);
    }

    return 0;
}
