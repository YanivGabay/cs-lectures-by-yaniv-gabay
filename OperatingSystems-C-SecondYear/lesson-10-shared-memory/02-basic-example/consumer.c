
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
            printf("Consumer: Buffer is empty, waiting...\n");
            sleep(1);
        }

        //if its full, "consume"
        if(shared_mem_ptr[0] == FULL)
        {
            printf("Consumer: Consuming data...\n");
            //copy the data from shared memory to local variable
            strcpy(str, shared_mem_ptr + 1);
            printf("Consumer: Consumed data: %s\n", str);
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
    printf("Consumer: Detached shared memory\n");

    return 0;



}