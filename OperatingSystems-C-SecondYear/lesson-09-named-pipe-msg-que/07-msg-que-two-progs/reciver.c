// receiver.c
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
int const MSG_TYPE_EXIT =  3;

struct my_msgbuf {
    long mtype; // Message type
    char mtext[256]; // Message data
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

    printf("Receiver: Waiting for messages...\n");

    while (1) {
        // Receive any message
        if (msgrcv(msgid, &buf, sizeof(buf.mtext), 0, 0) == -1) {
            perror("msgrcv");
            exit(EXIT_FAILURE);
        }

        // Check for exit message
        if (buf.mtype == MSG_TYPE_EXIT) {
            printf("Receiver: Exit message received. Exiting.\n");
            break;
        }

        printf("Receiver: Received message: %s\n", buf.mtext);
    }

    // Cleanup: Remove the message queue
    if (msgctl(msgid, IPC_RMID, NULL) == -1) {
        perror("msgctl");
        exit(EXIT_FAILURE);
    }

    return 0;
}
