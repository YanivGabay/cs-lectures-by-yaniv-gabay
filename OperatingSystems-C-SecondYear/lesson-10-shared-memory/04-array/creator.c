/*
 * creator.c — Shared array creator — allocates shared memory for array
 *
 * Key concepts: shmget with array size, shared memory initialization
 * Compile: gcc -o arr_create creator.c
 * Run:     ./prog
 */
// array_creator.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/ipc.h>

const char* SHM_KEY =  '0x2345'; // Unique key for shared memory
const int ARRAY_SIZE =  10 ; // Size of the integer array

int shmid;
int *shared_array;

void handle_sigint(int sig)
{
    printf("\n[Creator] Caught signal %d, detaching and removing shared memory (shmid=%d).\n", sig, shmid);
    shmdt(shared_array);
    shmctl(shmid, IPC_RMID, NULL);
    exit(EXIT_SUCCESS);
}

int main()
{
    printf("\n");
    printf("══════════════════════════════════════\n");
    printf("  Shared array creator — allocates s\n");
    printf("══════════════════════════════════════\n\n");

    signal(SIGINT, handle_sigint);
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

    printf("[Creator PID %d] Shared memory for integer array created and initialized (shmid=%d).\n", getpid(), shmid);
    printf("[Creator] Press Ctrl+C to terminate and clean up.\n");

    // Keep the creator running to maintain the shared memory
    while (1)
    {
        sleep(1);
    }

  

    return 0;
}
