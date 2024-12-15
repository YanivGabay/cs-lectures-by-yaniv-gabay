#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define QUEUE_KEY_PATH "/tmp"
#define PROJECT_ID 65

struct my_msgbuf {
    long mtype;
    char mtext[100];
};

int main() {
    key_t key;
    int msgid;
    pid_t pid;
    struct my_msgbuf buf;

    // Generate unique key
    key = ftok(QUEUE_KEY_PATH, PROJECT_ID);
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

    if (pid == 0) {  // Child Process
        // Receive message from parent (type 1)
        memset(&buf, 0, sizeof(buf));
        if (msgrcv(msgid, &buf, sizeof(buf.mtext), 1, 0) == -1) {
            perror("Child msgrcv type 1");
            exit(EXIT_FAILURE);
        }
        printf("Child received from parent: %s\n", buf.mtext);

        // Send response to parent (type 2)
        buf.mtype = 2;
        strcpy(buf.mtext, "Hello from Child!");
        if (msgsnd(msgid, &buf, sizeof(buf.mtext), 0) == -1) {
            perror("Child msgsnd type 2");
            exit(EXIT_FAILURE);
        }
        printf("Child sent to parent: %s\n", buf.mtext);

        exit(EXIT_SUCCESS);
    } else {         // Parent Process
        // Send message to child (type 1)
        buf.mtype = 1;
        strcpy(buf.mtext, "Hello from Parent!");
        if (msgsnd(msgid, &buf, sizeof(buf.mtext), 0) == -1) {
            perror("Parent msgsnd type 1");
            exit(EXIT_FAILURE);
        }
        printf("Parent sent to child: %s\n", buf.mtext);

        // Receive response from child (type 2)
        memset(&buf, 0, sizeof(buf));
        if (msgrcv(msgid, &buf, sizeof(buf.mtext), 2, 0) == -1) {
            perror("Parent msgrcv type 2");
            exit(EXIT_FAILURE);
        }
        printf("Parent received from child: %s\n", buf.mtext);

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
