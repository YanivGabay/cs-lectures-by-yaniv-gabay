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
    printf("\nCaught signal %d, detaching and removing shared memory.\n", sig);
    shmdt(pos);
    shmctl(shmid, IPC_RMID, NULL);
    exit(EXIT_SUCCESS);
}

int main()
{
  
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

    printf("Shared memory for Position created and initialized.\n");
    printf("Press Ctrl+C to terminate the creator.\n");

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
