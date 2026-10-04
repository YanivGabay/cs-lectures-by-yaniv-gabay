/*
 * scoreboard_creator.c — Scoreboard creator — shared struct with header file
 *
 * Key concepts: shmget, shared header file for struct definition
 * Compile: gcc -o sb_create scoreboard_creator.c
 * Run:     ./prog
 */
// scoreboard_creator.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> 
#include <sys/shm.h>
#include <sys/ipc.h>
#include "scoreboard.h"

#define SHM_KEY 0x1234 // Unique key for shared memory

int main() {
    printf("\n");
    printf("══════════════════════════════════════\n");
    printf("  Scoreboard creator — shared struct\n");
    printf("══════════════════════════════════════\n\n");
    int shmid;
    Scoreboard *scoreboard;

    // Create shared memory segment
    shmid = shmget(SHM_KEY, sizeof(Scoreboard), IPC_CREAT | 0666);
    if (shmid < 0) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    // Attach to shared memory
    scoreboard = (Scoreboard *) shmat(shmid, NULL, 0);
    if (scoreboard == (Scoreboard *) -1) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }

    // Initialize scoreboard
    scoreboard->total_players = 0;
    for(int i = 0; i < MAX_PLAYERS; i++) {
        scoreboard->players[i].player_id = -1; // -1 indicates empty slot
        strcpy(scoreboard->players[i].player_name, "Empty");
        scoreboard->players[i].score = 0;
    }

    printf("[Creator] Scoreboard initialized in shared memory.\n");

    // Detach from shared memory
    if (shmdt(scoreboard) == -1) {
        perror("shmdt");
        exit(EXIT_FAILURE);
    }

    return 0;
}
