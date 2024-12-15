#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>


// THIS PROGRAM DOESNT!!! DELETE THE MESSAGE QUEUE
// so we will have to do it manually

//ipcs -q
//example output:
/*
------ Message Queues --------
key        msqid      owner      perms      used-bytes   messages    
0x12345    123456     user       644        0            0           
*/

//removing:
//ipcrm -q 123456

// run again:
//ipcs -q



struct my_msgbuf {
    long mtype;
    char mtext[100];
};

int main() {
    key_t key;
    int msgid;
    struct my_msgbuf buf;

    // Generate unique key
      key = ftok("/tmp", 65); 
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

    printf("Message Queue created with msgid: %d\n", msgid);

    // Program ends without deleting the message queue
    return 0;
}
