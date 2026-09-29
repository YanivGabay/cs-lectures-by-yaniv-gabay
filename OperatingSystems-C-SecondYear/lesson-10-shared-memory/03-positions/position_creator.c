/*
 * position_creator.c — Position struct in shared memory — creator
 *
 * Key concepts: Shared memory with structs, shmget with sizeof
 * Compile: gcc -o pos_create position_creator.c
 * Run:     ./prog
 */
// position_creator.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <signal.h>

#define SHM_KEY 0x1234 // Unique key for shared memory
#define SHM_SIZE sizeof(Position)

typedef struct
{
    float x;
    float y;
} Position;

//global variables for signal handler
 int shmid;
 Position *pos;


void handle_sigint(int sig)
{
    printf("\n[Creator] Caught signal %d. Detaching and removing shared memory (shmid=%d).\n", sig, shmid);
    shmdt(pos);
    shmctl(shmid, IPC_RMID, NULL);
    exit(EXIT_SUCCESS);
}

int main()
{
    printf("\n");
    printf("══════════════════════════════════════\n");
    printf("  Position struct in shared memory —\n");
    printf("══════════════════════════════════════\n\n");
  
    signal(SIGINT, handle_sigint);
    // Create shared memory segment
    shmid = shmget(SHM_KEY, SHM_SIZE, IPC_CREAT | 0666);
    if (shmid < 0)
    {
        perror("shmget failed");
        exit(EXIT_FAILURE);
    }

    // Attach to shared memory
    pos = (Position *)shmat(shmid, NULL, 0);
    if (pos == (Position *)-1)
    {
        perror("shmat failed");
        // Remove shared memory segment
        shmctl(shmid, IPC_RMID, NULL);
        exit(EXIT_FAILURE);
    }

    // Initialize position
    pos->x = 0.0;
    pos->y = 0.0;

    printf("[Creator PID %d] Shared memory created (shmid=%d, key=0x%x).\n", getpid(), shmid, SHM_KEY);
    printf("[Creator] Position initialized to (%.1f, %.1f).\n", pos->x, pos->y);
    printf("[Creator] Press Ctrl+C to clean up and exit.\n");

    // Keep the creator running to maintain the shared memory
    while (1)
    {
        sleep(1);

       
    }

    // Detach and remove shared memory (unreachable in this example)
    shmdt(pos);
    shmctl(shmid, IPC_RMID, NULL);

    return 0;
}
