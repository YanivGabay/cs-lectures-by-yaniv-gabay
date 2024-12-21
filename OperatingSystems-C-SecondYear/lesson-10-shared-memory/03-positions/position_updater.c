// position_updater.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include <sys/shm.h>
#include <sys/ipc.h>
#include <signal.h>


#define SHM_KEY 0x1234 // Must match creator's key
#define SHM_SIZE sizeof(Position)

typedef struct
{
    float x;
    float y;
} Position;


int shmid;
Position *pos;

// Signal handler to detach shared memory upon termination
void handle_sigint(int sig) {
    if(shmdt(pos) == -1) {
        perror("shmdt failed");
    }
    printf("\nPosition updater detached and exiting.\n");
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

    printf("Position updater started. Updating position every second.\n");

    // Update position in a loop
    while(1) {
        // Simple movement logic: move diagonally
        pos->x += 1.0;
        pos->y += 1.0;

        printf("Updated Position: x = %.2f, y = %.2f\n", pos->x, pos->y);

        sleep(1);
    }

    // Detach from shared memory (unreachable in this example)
    shmdt(pos);

    return 0;
}
