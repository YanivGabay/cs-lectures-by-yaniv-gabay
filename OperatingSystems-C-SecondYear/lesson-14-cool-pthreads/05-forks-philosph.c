// File: ipc_dining_philosophers.c
// Compile with: gcc -Wall -o ipc_dining_philosophers ipc_dining_philosophers.c -lrt

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <fcntl.h>           // For O_CREAT, O_RDWR
#include <sys/stat.h>        // For mode constants
#include <sys/wait.h>
#include <sys/shm.h>      // For shmget, shmat
#include <sys/ipc.h>
#include <string.h>
#include <errno.h>
#include <time.h>

#define NUM_PHILOSOPHERS 5
#define EAT_COUNT 3

// Semaphore names for forks
const char *fork_names[NUM_PHILOSOPHERS] = {
    "/fork0_ipc",
    "/fork1_ipc",
    "/fork2_ipc",
    "/fork3_ipc",
    "/fork4_ipc"
};

// Shared memory key
#define SHM_KEY_PATH "/tmp"
#define SHM_KEY_ID 'z'

// Structure for dining status (optional)
typedef struct {
    int times_eaten[NUM_PHILOSOPHERS];
} dining_status_t;

// Function prototypes
void philosopher(int phil_id, sem_t **forks, dining_status_t *status);
void cleanup_semaphores();
void cleanup_shared_memory();

int main() {
    pid_t pid;
    sem_t *forks[NUM_PHILOSOPHERS];
    key_t shm_key;
    int shm_id;
    dining_status_t *dining_status;

    // Remove existing semaphores and shared memory if any
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        sem_unlink(fork_names[i]);
    }
    shm_unlink("/shm_dining_ipc");

    // Initialize semaphores for forks
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        forks[i] = sem_open(fork_names[i], O_CREAT, 0600, 1); // Each fork is a binary semaphore
        if (forks[i] == SEM_FAILED) {
            perror("sem_open failed for fork");
            // Cleanup any already opened semaphores
            for (int j = 0; j < i; j++) {
                sem_close(forks[j]);
                sem_unlink(fork_names[j]);
            }
            exit(EXIT_FAILURE);
        }
    }

    // Generate unique key for shared memory
    shm_key = ftok(SHM_KEY_PATH, SHM_KEY_ID);
    if (shm_key == -1) {
        perror("ftok failed");
        cleanup_semaphores();
        exit(EXIT_FAILURE);
    }

    // Create shared memory segment
    shm_id = shmget(shm_key, sizeof(dining_status_t), IPC_CREAT | 0600);
    if (shm_id == -1) {
        perror("shmget failed");
        cleanup_semaphores();
        exit(EXIT_FAILURE);
    }

    // Attach shared memory segment
    dining_status = (dining_status_t *)shmat(shm_id, NULL, 0);
    if (dining_status == (void *)-1) {
        perror("shmat failed");
        shmctl(shm_id, IPC_RMID, NULL);
        cleanup_semaphores();
        exit(EXIT_FAILURE);
    }

    // Initialize dining status
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        dining_status->times_eaten[i] = 0;
    }

    // Seed the random number generator
    srand(time(NULL));

    // Create philosopher processes
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pid = fork();
        if (pid < 0) {
            perror("fork failed for philosopher");
            // Cleanup before exiting
            shmdt(dining_status);
            shmctl(shm_id, IPC_RMID, NULL);
            cleanup_semaphores();
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            // Child process - Philosopher
            philosopher(i, forks, dining_status);
            // Detach shared memory and exit
            shmdt(dining_status);
            exit(EXIT_SUCCESS);
        }
    }

    // Parent process waits for all philosopher processes to finish
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        wait(NULL);
    }

    // Display dining status
    printf("\nDining Status:\n");
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        printf("Philosopher %d ate %d times.\n", i, dining_status->times_eaten[i]);
    }

    // Cleanup
    shmdt(dining_status);
    shmctl(shm_id, IPC_RMID, NULL);
    cleanup_semaphores();

    printf("Dining Philosophers simulation completed.\n");
    return EXIT_SUCCESS;
}

void philosopher(int phil_id, sem_t **forks, dining_status_t *status) {
    int left = phil_id;
    int right = (phil_id + 1) % NUM_PHILOSOPHERS;

    // To prevent deadlock, enforce resource ordering by always picking up the lower-numbered fork first
    int first = left < right ? left : right;
    int second = left < right ? right : left;

    for (int i = 0; i < EAT_COUNT; i++) {
        // Thinking
        printf("Philosopher %d is thinking.\n", phil_id);
        usleep((rand() % 500 + 500) * 1000); // 0.5 to 1 second

        // Picking up first fork
        printf("Philosopher %d is hungry and tries to pick up fork %d.\n", phil_id, first);
        if (sem_wait(forks[first]) == -1) {
            perror("sem_wait failed on first fork");
            exit(EXIT_FAILURE);
        }
        printf("Philosopher %d picked up fork %d.\n", phil_id, first);

        // Picking up second fork
        printf("Philosopher %d tries to pick up fork %d.\n", phil_id, second);
        if (sem_wait(forks[second]) == -1) {
            perror("sem_wait failed on second fork");
            // Release the first fork before exiting
            sem_post(forks[first]);
            exit(EXIT_FAILURE);
        }
        printf("Philosopher %d picked up fork %d.\n", phil_id, second);

        // Eating
        printf("Philosopher %d is eating.\n", phil_id);
        usleep((rand() % 500 + 500) * 1000); // 0.5 to 1 second
        status->times_eaten[phil_id]++;

        // Putting down second fork
        if (sem_post(forks[second]) == -1) {
            perror("sem_post failed on second fork");
            exit(EXIT_FAILURE);
        }
        printf("Philosopher %d put down fork %d.\n", phil_id, second);

        // Putting down first fork
        if (sem_post(forks[first]) == -1) {
            perror("sem_post failed on first fork");
            exit(EXIT_FAILURE);
        }
        printf("Philosopher %d put down fork %d.\n", phil_id, first);
    }

    printf("Philosopher %d has finished dining.\n", phil_id);
}

void cleanup_semaphores() {
    // Close and unlink all fork semaphores
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        sem_close(sem_open(fork_names[i], 0));
        sem_unlink(fork_names[i]);
    }
}
