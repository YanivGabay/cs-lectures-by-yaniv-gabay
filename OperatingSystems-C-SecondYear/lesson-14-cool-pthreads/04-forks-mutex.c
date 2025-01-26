// File: ipc_producer_consumer.c
// Compile with: gcc -Wall -o ipc_producer_consumer ipc_producer_consumer.c -lrt

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <fcntl.h>    // For O_CREAT, O_RDWR
#include <sys/stat.h> // For mode constants
#include <sys/wait.h>

#include <sys/ipc.h>
#include <string.h>
#include <errno.h>
#include <sys/shm.h>
#define BUFFER_SIZE 5
#define NUM_PRODUCERS 2
#define NUM_CONSUMERS 2
#define ITEMS_PER_PRODUCER 10

// Semaphore names
#define SEM_MUTEX_NAME "/sem_mutex_ipc"
#define SEM_EMPTY_NAME "/sem_empty_ipc"
#define SEM_FULL_NAME "/sem_full_ipc"

// Shared memory key
#define SHM_KEY_PATH "/tmp"
#define SHM_KEY_ID 'y'

// Structure for shared buffer
typedef struct
{
    int buffer[BUFFER_SIZE];
    int in;
    int out;
} shared_buffer_t;

// Function prototypes
void producer(int producer_id, shared_buffer_t *shared_buffer, sem_t *mutex, sem_t *empty, sem_t *full);
void consumer(int consumer_id, shared_buffer_t *shared_buffer, sem_t *mutex, sem_t *empty, sem_t *full);

// global variables
sem_t *mutex, *empty, *full;
shared_buffer_t *shared_buffer;

void unlinker(int amount)
{
    for (int i = 0; i < amount; i++)
    {
        sem_unlink(SEM_MUTEX_NAME);
        sem_unlink(SEM_EMPTY_NAME);
        sem_unlink(SEM_FULL_NAME);
    }
}
void closer(sem_t **sem, int amount)
{
    for (int i = 0; i < amount; i++)
    {
        sem_close(sem[i]);
    }
}
void closeAll()
{
    sem_t *sems[] = {mutex, empty, full};
    closer(sems, 3);
    unlinker(3);
}
int main()
{
    pid_t pid;

    key_t shm_key;
    int shm_id;

    // Remove existing semaphores and shared memory if any
    unlinker(3);

    // Create named semaphores
    mutex = sem_open(SEM_MUTEX_NAME, O_CREAT, 0600, 1); // Binary semaphore for mutual exclusion
    if (mutex == SEM_FAILED)
    {
        perror("sem_open failed for mutex");
        exit(EXIT_FAILURE);
    }

    empty = sem_open(SEM_EMPTY_NAME, O_CREAT, 0600, BUFFER_SIZE); // Counting semaphore for empty slots
    if (empty == SEM_FAILED)
    {
        perror("sem_open failed for empty");
        sem_close(mutex);
        closer(&mutex, 1);
        unlinker(1);
        exit(EXIT_FAILURE);
    }

    full = sem_open(SEM_FULL_NAME, O_CREAT, 0600, 0); // Counting semaphore for full slots
    if (full == SEM_FAILED)
    {
        perror("sem_open failed for full");
        sem_t *sems[] = {mutex, empty};
        closer(sems, 2);

        unlinker(2);
        exit(EXIT_FAILURE);
    }

    // Generate unique key for shared memory
    shm_key = ftok(SHM_KEY_PATH, SHM_KEY_ID);
    if (shm_key == -1)
    {
        closeAll();
        exit(EXIT_FAILURE);
    }

    // Create shared memory segment
    shm_id = shmget(shm_key, sizeof(shared_buffer_t), IPC_CREAT | 0600);
    if (shm_id == -1)
    {
        perror("shmget failed");
        closeAll();
        exit(EXIT_FAILURE);
    }

    // Attach shared memory segment
    shared_buffer = (shared_buffer_t *)shmat(shm_id, NULL, 0);
    if (shared_buffer == (void *)-1)
    {
        perror("shmat failed");
        shmctl(shm_id, IPC_RMID, NULL);
        closeAll();
        exit(EXIT_FAILURE);
    }

    // Initialize shared buffer
    shared_buffer->in = 0;
    shared_buffer->out = 0;
    memset(shared_buffer->buffer, 0, sizeof(shared_buffer->buffer));

    // Spawn producer processes
    for (int i = 0; i < NUM_PRODUCERS; i++)
    {
        pid = fork();
        if (pid < 0)
        {
            perror("fork failed for producer");
            // Cleanup before exiting
            shmdt(shared_buffer);
            shmctl(shm_id, IPC_RMID, NULL);
            closeAll();
            exit(EXIT_FAILURE);
        }
        else if (pid == 0)
        {
            // Child process - Producer
            producer(i + 1, shared_buffer, mutex, empty, full);
            // Detach shared memory and exit
            shmdt(shared_buffer);
            exit(EXIT_SUCCESS);
        }
    }

    // Spawn consumer processes
    for (int i = 0; i < NUM_CONSUMERS; i++)
    {
        pid = fork();
        if (pid < 0)
        {
            perror("fork failed for consumer");
            // Cleanup before exiting
            shmdt(shared_buffer);
            shmctl(shm_id, IPC_RMID, NULL);
            closeAll();
            exit(EXIT_FAILURE);
        }
        else if (pid == 0)
        {
            // Child process - Consumer
            consumer(i + 1, shared_buffer, mutex, empty, full);
            // Detach shared memory and exit
            shmdt(shared_buffer);
            exit(EXIT_SUCCESS);
        }
    }

    // Parent process waits for all children to finish
    for (int i = 0; i < NUM_PRODUCERS + NUM_CONSUMERS; i++)
    {
        wait(NULL);
    }

    // Detach and remove shared memory
    shmdt(shared_buffer);
    shmctl(shm_id, IPC_RMID, NULL);

    // Clean up semaphores
    closeAll();
    printf("Producer-Consumer simulation completed.\n");
    return EXIT_SUCCESS;
}

void producer(int producer_id, shared_buffer_t *shared_buffer, sem_t *mutex, sem_t *empty, sem_t *full)
{
    srand(getpid()); // Seed the random number generator

    for (int i = 0; i < ITEMS_PER_PRODUCER; i++)
    {
        int item = producer_id * 100 + i; // Unique item

        // Wait (lock) on 'empty' semaphore to ensure there's space
        if (sem_wait(empty) == -1)
        {
            perror("sem_wait failed on empty");
            exit(EXIT_FAILURE);
        }

        // Lock the mutex before accessing the buffer
        if (sem_wait(mutex) == -1)
        {
            perror("sem_wait failed on mutex");
            sem_post(empty); // Release 'empty' if mutex lock fails
            exit(EXIT_FAILURE);
        }

        // Critical section: Add item to buffer
        shared_buffer->buffer[shared_buffer->in] = item;
        printf("Producer %d produced item %d at position %d.\n", producer_id, item, shared_buffer->in);
        shared_buffer->in = (shared_buffer->in + 1) % BUFFER_SIZE;

        // Unlock the mutex after accessing the buffer
        if (sem_post(mutex) == -1)
        {
            perror("sem_post failed on mutex");
            exit(EXIT_FAILURE);
        }

        // Signal that buffer has at least one full slot
        if (sem_post(full) == -1)
        {
            perror("sem_post failed on full");
            exit(EXIT_FAILURE);
        }

        // Simulate production time
        usleep(100000); // 100 ms
    }

    printf("Producer %d finished producing.\n", producer_id);
}

void consumer(int consumer_id, shared_buffer_t *shared_buffer, sem_t *mutex, sem_t *empty, sem_t *full)
{
    for (int i = 0; i < ITEMS_PER_PRODUCER; i++)
    {
        // Wait (lock) on 'full' semaphore to ensure there's at least one item
        if (sem_wait(full) == -1)
        {
            perror("sem_wait failed on full");
            exit(EXIT_FAILURE);
        }

        // Lock the mutex before accessing the buffer
        if (sem_wait(mutex) == -1)
        {
            perror("sem_wait failed on mutex");
            sem_post(full); // Release 'full' if mutex lock fails
            exit(EXIT_FAILURE);
        }

        // Critical section: Remove item from buffer
        int item = shared_buffer->buffer[shared_buffer->out];
        if (item == -1)
        { // Poison pill to signal termination (optional)
            // Unlock the mutex and 'full' semaphore before exiting
            if (sem_post(mutex) == -1)
            {
                perror("sem_post failed on mutex during termination");
                exit(EXIT_FAILURE);
            }
            if (sem_post(full) == -1)
            {
                perror("sem_post failed on full during termination");
                exit(EXIT_FAILURE);
            }
            break;
        }
        printf("Consumer %d consumed item %d from position %d.\n", consumer_id, item, shared_buffer->out);
        shared_buffer->out = (shared_buffer->out + 1) % BUFFER_SIZE;

        // Unlock the mutex after accessing the buffer
        if (sem_post(mutex) == -1)
        {
            perror("sem_post failed on mutex");
            exit(EXIT_FAILURE);
        }

        // Signal that buffer has at least one empty slot
        if (sem_post(empty) == -1)
        {
            perror("sem_post failed on empty");
            exit(EXIT_FAILURE);
        }

        // Simulate consumption time
        usleep(150000); // 150 ms
    }

    printf("Consumer %d finished consuming.\n", consumer_id);
}
