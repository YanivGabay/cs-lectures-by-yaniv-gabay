/*
 * position_viewer.c — Position struct viewer — reads shared memory
 *
 * Key concepts: Polling shared memory, real-time data viewing
 * Compile: gcc -o pos_view position_viewer.c
 * Run:     ./prog
 */
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
    printf("\n[Viewer] Detached and exiting.\n");
    exit(0);
}

int main() {
    printf("\n");
    printf("══════════════════════════════════════\n");
    printf("  Position struct viewer — reads sha\n");
    printf("══════════════════════════════════════\n\n");
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

    printf("[Viewer PID %d] Started. Reading position every second.\n", getpid());

    // Read position in a loop
    while(1) {
        printf("[Viewer] Position: x = %.2f, y = %.2f\n", pos->x, pos->y);
        sleep(1);
    }

    // Detach from shared memory (unreachable in this example)
    shmdt(pos);

    return 0;
}
