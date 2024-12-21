// scoreboard.h
#ifndef SCOREBOARD_H
#define SCOREBOARD_H

#define MAX_PLAYERS 10
#define PLAYER_NAME_LEN 50

typedef struct {
    int player_id;
    char player_name[PLAYER_NAME_LEN];
    int score;
} Player;

typedef struct {
    int total_players;
    Player players[MAX_PLAYERS];
} Scoreboard;

#endif // SCOREBOARD_H
