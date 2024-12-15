#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

struct my_msgbuf {
    long mtype;
    char mtext[100];
};
int const ALLOWED_TYPES[] = {1, 2, 3};

int main() {
    key_t key;
    int msgid;
    pid_t pid;
    struct my_msgbuf buf;

    // Generate unique key
    key = ftok("progfile", 65);
    if (key == -1) {
        perror("ftok");
        exit(EXIT_FAILURE);
    }

    // Create message queue
    msgid = msgget(key, 0666 | IPC_CREAT);
    if (msgid == -1) {
        perror("msgget");
        exit(EXIT_FAILURE);
    }

    pid = fork();
    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {  // Child Process: Receiver for type 2
        memset(&buf, 0, sizeof(buf));

        // Receive message of type 2
        if (msgrcv(msgid, &buf, sizeof(buf.mtext), ALLOWED_TYPES[1], 0) == -1) {
            perror("msgrcv");
            exit(EXIT_FAILURE);
        }

        printf("Child received message of type 2: %s\n", buf.mtext);
        exit(EXIT_SUCCESS);
    } else {         // Parent Process: Sender
        // Send message of type 1
        buf.mtype = 1;
        strcpy(buf.mtext, "Message Type 1");
        if (msgsnd(msgid, &buf, sizeof(buf.mtext), 0) == -1) {
            perror("msgsnd type 1");
            exit(EXIT_FAILURE);
        }
        printf("Parent sent message of type 1: %s\n", buf.mtext);

        // Send message of type 2
        buf.mtype = 2;
        strcpy(buf.mtext, "Message Type 2");
        if (msgsnd(msgid, &buf, sizeof(buf.mtext), 0) == -1) {
            perror("msgsnd type 2");
            exit(EXIT_FAILURE);
        }
        printf("Parent sent message of type 2: %s\n", buf.mtext);

        // Wait for child to finish
        wait(NULL);

        // Remove message queue
        if (msgctl(msgid, IPC_RMID, NULL) == -1) {
            perror("msgctl");
            exit(EXIT_FAILURE);
        }
    }

    return 0;
}
