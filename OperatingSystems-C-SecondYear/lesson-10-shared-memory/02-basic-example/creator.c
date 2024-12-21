
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

    //this program will create a shared memory segment
    //https://man7.org/linux/man-pages/man2/shmget.2.html
    // we can see all flags and what they do
    if((shmid = shmget(key, SHM_SIZE, IPC_CREAT | IPC_EXCL | 0666)) < 0) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    //now we created the shared memeory
    // but we need to create a ptr to access it
    shared_mem_ptr = (char *) shmat(shmid, NULL, 0);
    //we know that each cell represents a char
    //that why we did the casting
    if (shared_mem_ptr == (char *) -1) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }

    //now, we will use the [0] cell as an indicator
    //to know if the shared memory is empty or not

    shared_mem_ptr[0] = EMPTY;

    printf("now we can read input from the user\n");

    while (1) {
        printf("Enter a string: ");
        fgets(str, MAX_STRING_SIZE, stdin);
        //remove the newline character
        str[strcspn(str, "\n")] = 0;

        //check if the shared memory is full
        while (shared_mem_ptr[0] == FULL) {
            printf("The shared memory is full. Please wait.\n");
            sleep(1);
            
        }

        //copy the string to the shared memory
        strcpy(shared_mem_ptr + 1, str);
        shared_mem_ptr[0] = FULL;

        printf("String sent to shared memory: %s\n", str);

        //check if the user wants to exit
        if (strcmp(str, "exit") == 0) {
            break;
        }
    }
    //we send the other process an exit message
    //so we want to sleep a little bit 
    //to make sure the other process reads the message
    // and detaches from the shared memory
    sleep(2);
    //now we need to clean up
    //detach the ptr
    if(shmdt(shared_mem_ptr) < 0) {
        perror("shmdt");
        exit(EXIT_FAILURE);
    }
    //now we need to remove the shared memory
    if(shmctl(shmid, IPC_RMID, NULL) < 0) {
        perror("shmctl");
        exit(EXIT_FAILURE);
    }
    //https://man7.org/linux/man-pages/man2/shmctl.2.html
    /*
    IPC_RMID
              Mark the segment to be destroyed.  The segment will
              actually be destroyed only after the last process detaches
              it (i.e., when the shm_nattch member of the associated
              structure shmid_ds is zero).  The caller must be the owner
              or creator of the segment, or be privileged.  The buf
              argument is ignored.

              If a segment has been marked for destruction, then the
              (nonstandard) SHM_DEST flag of the shm_perm.mode field in
              the associated data structure retrieved by IPC_STAT will
              be set.

              The caller must ensure that a segment is eventually
              destroyed; otherwise its pages that were faulted in will
              remain in memory or swap.
*/


    return 0;
}
