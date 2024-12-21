// scoreboard_updater.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> 
#include <sys/shm.h>
#include <sys/ipc.h>
#include <signal.h>
#include "scoreboard.h"

#define SHM_KEY 0x1234 // Must match creator's key

int shmid;
Scoreboard *scoreboard;
int player_index = -1;

// Signal handler to detach shared memory upon termination
void handle_sigint(int sig) {
    if(scoreboard != (Scoreboard *) -1) {
        shmdt(scoreboard);
    }
    printf("\nPlayer detached and exiting.\n");
    exit(0);
}

int main(int argc, char *argv[]) {
    if(argc != 2) {
        fprintf(stderr, "Usage: %s <Player Name>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char *player_name = argv[1];

    // Register signal handler
    signal(SIGINT, handle_sigint);

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

    // Register player
    for(int i = 0; i < MAX_PLAYERS; i++) {
        if(scoreboard->players[i].player_id == -1) {
            scoreboard->players[i].player_id = getpid();
            strncpy(scoreboard->players[i].player_name, player_name, PLAYER_NAME_LEN-1);
            scoreboard->players[i].player_name[PLAYER_NAME_LEN-1] = '\0';
            scoreboard->players[i].score = 0;
            player_index = i;
            scoreboard->total_players++;
            printf("Player '%s' registered with ID %d at index %d.\n", player_name, getpid(), i);
            break;
        }
    }

    if(player_index == -1) {
        printf("No available slots for new players.\n");
        shmdt(scoreboard);
        exit(EXIT_FAILURE);
    }

    // Simulate score updates
    while(1) {
        sleep(2); // Wait for 2 seconds

        // Increment score randomly
        int increment = rand() % 10;
        scoreboard->players[player_index].score += increment;
        printf("Player '%s' (ID %d) scored %d points. Total: %d\n",
               scoreboard->players[player_index].player_name,
               scoreboard->players[player_index].player_id,
               increment,
               scoreboard->players[player_index].score);
    }

    // Detach (unreachable code due to infinite loop)
    shmdt(scoreboard);

    return 0;
}
