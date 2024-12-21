// position_viewer.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include <sys/shm.h>
#include <sys/ipc.h>
#include <signal.h>
#include "position.h"

#define SHM_KEY 0x1234 // Must match creator's key
#define SHM_SIZE sizeof(Position)

int shmid;
Position *pos;

// Signal handler to detach shared memory upon termination
void handle_sigint(int sig) {
    if(shmdt(pos) == -1) {
        perror("shmdt failed");
    }
    printf("\nPosition viewer detached and exiting.\n");
    exit(0);
}

int main() {
    // Register signal handler
    signal(SIGINT, handle_sigint);

    // Access shared memory
    shmid = shmget(SHM_KEY, SHM_SIZE, 0666);
    if(shmid < 0) {
        perror("shmget failed");
        exit(EXIT_FAILURE);
    }

    // Attach to shared memory
    pos = (Position *) shmat(shmid, NULL, 0);
    if(pos == (Position *) -1) {
        perror("shmat failed");
        exit(EXIT_FAILURE);
    }

    printf("Position viewer started. Reading position every second.\n");

    // Read position in a loop
    while(1) {
        printf("Current Position: x = %.2f, y = %.2f\n", pos->x, pos->y);
        sleep(1);
    }

    // Detach from shared memory (unreachable in this example)
    shmdt(pos);

    return 0;
}
