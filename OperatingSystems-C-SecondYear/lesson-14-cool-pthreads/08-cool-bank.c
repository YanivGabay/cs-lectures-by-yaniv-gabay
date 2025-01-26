// File: thread_safe_bank.c
// Compile with: gcc -Wall -pthread -o thread_safe_bank thread_safe_bank.c

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_THREADS 6
#define OPERATIONS_PER_THREAD 1000

// Shared bank account balance
long long account_balance = 0;

// Mutex to protect the account balance
pthread_mutex_t account_mutex;

// Function for deposit operations
void* deposit(void* arg) {
    for(int i = 0; i < OPERATIONS_PER_THREAD; i++) {
        // Lock the mutex before modifying the balance
        pthread_mutex_lock(&account_mutex);
        
        // Critical section: Deposit $1
        account_balance += 1;
        //p//rintf("Deposited $1\n");
        // Unlock the mutex after modification
        pthread_mutex_unlock(&account_mutex);
    }
    printf("Thread %ld finished deposit\n", pthread_self());
    pthread_exit(NULL);
}

// Function for withdrawal operations
void* withdraw(void* arg) {
    for(int i = 0; i < OPERATIONS_PER_THREAD; i++) {
        // Lock the mutex before modifying the balance
        pthread_mutex_lock(&account_mutex);
        
        // Critical section: Withdraw $1 if possible
        
            account_balance -= 1;
       
       // printf("Withdrew $1\n");
        // Unlock the mutex after modification
        pthread_mutex_unlock(&account_mutex);
    }
    printf("Thread %ld finished withdraw\n", pthread_self());
    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NUM_THREADS];
    int rc;

    // Initialize the mutex
    if(pthread_mutex_init(&account_mutex, NULL) != 0) {
        perror("Mutex initialization failed");
        exit(EXIT_FAILURE);
    }

    // Create threads: half will deposit, half will withdraw
    for(long t = 0; t < NUM_THREADS; t++) {
        if(t % 2 == 0) {
            rc = pthread_create(&threads[t], NULL, deposit, NULL);
        } else {
            rc = pthread_create(&threads[t], NULL, withdraw, NULL);
        }
        if(rc) {
            fprintf(stderr, "Error: Unable to create thread %ld, return code %d\n", t, rc);
            exit(EXIT_FAILURE);
        }
    }

    // Wait for all threads to finish
    for(int t = 0; t < NUM_THREADS; t++) {
        pthread_join(threads[t], NULL);
    }

    // Destroy the mutex
    pthread_mutex_destroy(&account_mutex);

    // Display the final account balance
    printf("Final account balance: %lld\n", account_balance);
    // Expected balance: Number of deposit threads * OPERATIONS_PER_THREAD - Number of withdrawal threads * OPERATIONS_PER_THREAD
    printf("Expected balance: %d\n", (NUM_THREADS / 2) * OPERATIONS_PER_THREAD - (NUM_THREADS / 2) * OPERATIONS_PER_THREAD);
    return 0;
}
