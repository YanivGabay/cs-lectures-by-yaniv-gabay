// array_consumer.c
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

// Signal handler to detach shared memory upon termination
void handle_sigint(int sig) {
    if(shmdt(shared_array) == -1) {
        perror("shmdt failed");
    }
    printf("\nArray consumer detached and exiting.\n");
    exit(0);
}

int main() {
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

    printf("Array consumer started. Reading array every second.\n");

    // Read array in a loop
    while(1) {
        printf("Current Array State:\n");
        for(int i = 0; i < ARRAY_SIZE; i++) {
            printf("shared_array[%d] = %d\n", i, shared_array[i]);
        }
        printf("-------------------------\n");
        sleep(1);
    }

    // Detach from shared memory (unreachable in this example)
    shmdt(shared_array);

    return 0;
}
