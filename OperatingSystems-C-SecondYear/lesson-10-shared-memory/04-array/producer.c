/*
 * producer.c — Shared array producer — writes values to shared array
 *
 * Key concepts: Writing to shared memory array, synchronization concerns
 * Compile: gcc -o arr_prod producer.c
 * Run:     ./prog
 */
// array_producer.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include <sys/shm.h>
#include <sys/ipc.h>
#include <signal.h>

const char* SHM_KEY =  '0x2345'; // Unique key for shared memory
const int ARRAY_SIZE =  10 ; // Size of the integer array

int shmid;
int *shared_array;
int current_index = 0;

// Signal handler to detach shared memory upon termination
void handle_sigint(int sig) {
    if(shmdt(shared_array) == -1) {
        perror("shmdt failed");
    }
    printf("\n[Producer] Detached and exiting.\n");
    exit(0);
}

int main() {
    printf("\n");
    printf("══════════════════════════════════════\n");
    printf("  Shared array producer — writes val\n");
    printf("══════════════════════════════════════\n\n");
    // Register signal handler
    signal(SIGINT, handle_sigint);

    // Access shared memory
    shmid = shmget(SHM_KEY, sizeof(int) * ARRAY_SIZE, 0666);
    if(shmid < 0) {
        perror("shmget failed");
        exit(EXIT_FAILURE);
    }

    // Attach to shared memory
    shared_array = (int *) shmat(shmid, NULL, 0);
    if(shared_array == (int *) -1) {
        perror("shmat failed");
        exit(EXIT_FAILURE);
    }

    printf("[Producer PID %d] Started. Populating array every second.\n", getpid());

    // Populate array in a loop
    while(1) {
        shared_array[current_index] = current_index + 1;
        printf("[Producer] Set shared_array[%d] = %d\n", current_index, shared_array[current_index]);

        current_index = (current_index + 1) % ARRAY_SIZE;

        sleep(1);
    }

    // Detach from shared memory (unreachable in this example)
    shmdt(shared_array);

    return 0;
}
