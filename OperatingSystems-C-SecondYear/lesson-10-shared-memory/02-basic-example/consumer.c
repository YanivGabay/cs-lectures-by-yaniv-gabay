/*
 * consumer.c — Shared memory consumer — reading from shared segment
 *
 * Key concepts: shmget, shmat, reading shared data, shmdt
 * Compile: gcc -o consumer consumer.c
 * Run:     ./prog
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> 
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>

const int MAX_STRING_SIZE = 100;
const int SHM_SIZE = MAX_STRING_SIZE + 1;
const char EMPTY = '-';
const char FULL = '+';


int main(int argc, char const *argv[])
{
    printf("\n");
    printf("══════════════════════════════════════\n");
    printf("  Shared memory consumer — reading f\n");
    printf("══════════════════════════════════════\n\n");
    key_t key;
    int shmid;
    char *shared_mem_ptr;

    char str[MAX_STRING_SIZE];

    //first create key using ftok
    key = ftok("/tmp", 65);
    //check if key is created successfully
    if (key == -1) {
        perror("ftok");
        exit(EXIT_FAILURE);
    }

    // now we want to get the shared memory the creator has created
    if((shmid = shmget(key, SHM_SIZE, 0666)) < 0) { //we can pass 0 instead of shm_size
    //that will just get the shared memory , and might be even better
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    //now create a pointer to the shared memory
    if((shared_mem_ptr = shmat(shmid, NULL, 0)) == (char *) -1) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }

    while(1)
    {
        //if its empty,"wait"
        while(shared_mem_ptr[0] == EMPTY)
        {
            printf("[Consumer PID %d] Buffer empty — waiting for creator...\n", getpid());
            sleep(1);
        }

        //if its full, "consume"
        if(shared_mem_ptr[0] == FULL)
        {
            strcpy(str, shared_mem_ptr + 1);
            printf("[Consumer PID %d] Read from shared memory: \"%s\"\n", getpid(), str);
            //set the shared memory to empty
            shared_mem_ptr[0] = EMPTY;
        }

        //if the data is "exit", then exit
        if(strcmp(str, "exit") == 0)
        {
            break;
        }
    }

    //if we got exit, we want to first detach the ptr, and quit, hopefully the creator
    //will delete the shared memory
    if(shmdt(shared_mem_ptr) == -1) {
        perror("shmdt");
        exit(EXIT_FAILURE);
    }
    printf("[Consumer] Detached from shared memory. Exiting.\n");

    return 0;



}