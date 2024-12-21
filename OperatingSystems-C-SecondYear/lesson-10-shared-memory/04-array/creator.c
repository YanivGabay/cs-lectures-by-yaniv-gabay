// array_creator.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/ipc.h>

#define SHM_KEY 0x2345 // Unique key for shared memory
#define ARRAY_SIZE 10  // Size of the integer array

int shmid;
int *shared_array;

void handle_sigint(int sig)
{
    printf("\nCaught signal %d, detaching and removing shared memory.\n", sig);
    shmdt(shared_array);
    shmctl(shmid, IPC_RMID, NULL);
    exit(EXIT_SUCCESS);
}

int main()
{

    // Create shared memory segment
    shmid = shmget(SHM_KEY, sizeof(int) * ARRAY_SIZE, IPC_CREAT | 0666);
    if (shmid < 0)
    {
        perror("shmget failed");
        exit(EXIT_FAILURE);
    }

    // Attach to shared memory
    shared_array = (int *)shmat(shmid, NULL, 0);
    if (shared_array == (int *)-1)
    {
        perror("shmat failed");
        // Remove shared memory segment
        shmctl(shmid, IPC_RMID, NULL);
        exit(EXIT_FAILURE);
    }

    // Initialize array to zero
    for (int i = 0; i < ARRAY_SIZE; i++)
    {
        shared_array[i] = 0;
    }

    printf("Shared memory for integer array created and initialized.\n");
    printf("Press Ctrl+C to terminate the creator.\n");

    // Keep the creator running to maintain the shared memory
    while (1)
    {
        sleep(1);
    }

  

    return 0;
}
