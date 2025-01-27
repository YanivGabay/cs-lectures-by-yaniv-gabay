#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_THREADS 20

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int condition_met = 0;

void* thread_func(void* arg) {
    long id = (long)arg;

    pthread_mutex_lock(&mutex);
    while (!condition_met) {
        pthread_cond_wait(&cond, &mutex);
        //WHEN we broadcast
        //all threads will be woken up
        //despite that, each one will try 
        // to acquire the mutex
        printf("Thread %ld woke up.\n", id);
    }
    pthread_mutex_unlock(&mutex);

    printf("Thread %ld proceeding.\n", id);
    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];
    
    // Create threads that will wait on the condition variable
    for(long i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, thread_func, (void*)i);
    }
    
    sleep(2); // Simulate some work
    
    // Wake up all threads at once using pthread_cond_broadcast
    pthread_mutex_lock(&mutex);
    condition_met = 1;
    pthread_cond_broadcast(&cond); // Wake up all waiting threads
    pthread_mutex_unlock(&mutex);
    
    // Join threads
    for(int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);

    return 0;
}
