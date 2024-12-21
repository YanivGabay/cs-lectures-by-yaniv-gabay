// scoreboard_viewer.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include <sys/shm.h>
#include <sys/ipc.h>
#include "scoreboard.h"

#define SHM_KEY 0x1234 // Must match creator's key

int main() {
    int shmid;
    Scoreboard *scoreboard;

    // Access shared memory
    shmid = shmget(SHM_KEY, sizeof(Scoreboard), 0666);
    if (shmid < 0) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    scoreboard = (Scoreboard *) shmat(shmid, NULL, 0);
    if (scoreboard == (Scoreboard *) -1) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }

    printf("Viewing Scoreboard:\n");

    while(1) {
        system("clear"); // Clear the terminal for better readability

        printf("=== Game Scoreboard ===\n");
        printf("Total Players: %d\n\n", scoreboard->total_players);
        printf("%-5s %-20s %-10s\n", "ID", "Player Name", "Score");
        printf("----------------------------------------\n");

        for(int i = 0; i < MAX_PLAYERS; i++) {
            if(scoreboard->players[i].player_id != -1) {
                printf("%-5d %-20s %-10d\n",
                       scoreboard->players[i].player_id,
                       scoreboard->players[i].player_name,
                       scoreboard->players[i].score);
            }
        }

        printf("\nPress Ctrl+C to exit.\n");

        sleep(3); // Refresh every 3 seconds
    }

    // Detach (unreachable code due to infinite loop)
    shmdt(scoreboard);

    return 0;
}
