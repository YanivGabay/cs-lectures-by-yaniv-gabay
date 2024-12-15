/// includes needed for the program
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MSG_SIZE 100

//https://man7.org/linux/man-pages/man3/msgrcv.3p.html

int const ALLOWED_TYPES[] = {1, 2, 3};
//we HAVE to use define for msgbuf size
// cus c doesnt like const inside of structs
// cus they arent compile time constants for him
struct my_msgbuf {
    long mtype;       // Message type HAS TO BE LONG
    char mtext[MSG_SIZE];  // Message data
    // THIS CAN VARIE IN TYPE AND SIZE
};


int main() {
    key_t key;
    int msgid;
    pid_t pid;
    struct my_msgbuf buf;

    // Generate unique key using ftok
    key = ftok("/tmp", 65); 
    if (key == -1) {
        perror("ftok");
        exit(EXIT_FAILURE);
    }

    // Create message queue and return identifier
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

    if (pid == 0) {  // Child Process: Receiver
        // Initialize buffer
        memset(&buf, 0, sizeof(buf));

        // Receive message of type 1
        if (msgrcv(msgid, &buf, sizeof(buf.mtext), ALLOWED_TYPES[0], 0) == -1) {
            perror("msgrcv");
            exit(EXIT_FAILURE);
        }

        printf("Child received message: %s\n", buf.mtext);
        exit(EXIT_SUCCESS);
    } else {         // Parent Process: Sender
        // Prepare message
        buf.mtype = 1;
        strcpy(buf.mtext, "Hello from Parent!");

        // Send message
        if (msgsnd(msgid, &buf, sizeof(buf.mtext), 0) == -1) {
            perror("msgsnd");
            exit(EXIT_FAILURE);
        }

        printf("Parent sent message: %s\n", buf.mtext);

        // Wait for child to finish
        wait(NULL);

        // Remove message queue
        //VERY IMPORTANT TO DO THIS
        if (msgctl(msgid, IPC_RMID, NULL) == -1) {
            perror("msgctl");
            exit(EXIT_FAILURE);
        }
    }

    return 0;
}


