// sender.c
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

    // Generate the same unique key
    key = ftok("progfile", 65);
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

    printf("Sender: Enter messages to send. Type 'exit' to quit.\n");

    while (1) {
        printf("Sender: ");
        fgets(buf.mtext, sizeof(buf.mtext), stdin);
        buf.mtext[strcspn(buf.mtext, "\n")] = '\0'; // Remove newline

        if (strcmp(buf.mtext, "exit") == 0) {
            buf.mtype = MSG_TYPE_EXIT;
        } else {
            buf.mtype = 1; // Default message type
        }

        // Send the message
        if (msgsnd(msgid, &buf, strlen(buf.mtext) + 1, 0) == -1) {
            perror("msgsnd");
            exit(EXIT_FAILURE);
        }

        if (buf.mtype == MSG_TYPE_EXIT) {
            printf("Sender: Exit message sent. Exiting.\n");
            break;
        }
    }

    return 0;
}
