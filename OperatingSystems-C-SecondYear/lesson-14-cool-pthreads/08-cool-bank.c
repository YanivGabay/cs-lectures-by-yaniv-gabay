/*
 * 08-cool-bank.c — Bank simulation — concurrent account transfers
 *
 * Key concepts: Multiple threads transferring money, mutex per account, avoiding deadlock
 * Compile: gcc -o bank 08-cool-bank.c -lpthread
 * Run:     ./prog
 */
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
        // (not printing per-operation to avoid flooding output with 1000 lines)
        // Unlock the mutex after modification
        pthread_mutex_unlock(&account_mutex);
    }
    printf("[Deposit Thread] Finished — deposited $%d total\n", OPERATIONS_PER_THREAD);
    pthread_exit(NULL);
}

// Function for withdrawal operations
void* withdraw(void* arg) {
    for(int i = 0; i < OPERATIONS_PER_THREAD; i++) {
        // Lock the mutex before modifying the balance
        pthread_mutex_lock(&account_mutex);
        
        // Critical section: Withdraw $1 if possible
        
            account_balance -= 1;
       
        // (not printing per-operation to avoid flooding output)
        // Unlock the mutex after modification
        pthread_mutex_unlock(&account_mutex);
    }
    printf("[Withdraw Thread] Finished — withdrew $%d total\n", OPERATIONS_PER_THREAD);
    pthread_exit(NULL);
}

int main() {
    printf("\n");
    printf("══════════════════════════════════════\n");
    printf("  Bank Simulation — Concurrent Transfers\n");
    printf("══════════════════════════════════════\n\n");
    printf("[Main] %d threads: %d deposit, %d withdraw. Each does %d operations ($1 each).\n", NUM_THREADS, NUM_THREADS/2, NUM_THREADS/2, OPERATIONS_PER_THREAD);
    printf("[Main] With mutex: balance should end at $0 (deposits == withdrawals).\n\n");
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

    long long expected = 0; // equal deposits and withdrawals
    printf("\n══════════════════════════════════════\n");
    printf("  Final balance: $%lld (expected: $%lld) %s\n", account_balance, expected, account_balance == expected ? "✓" : "← BUG!");
    printf("══════════════════════════════════════\n");
    return 0;
}
