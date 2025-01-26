#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

//showing that signals will be lost

#define NUM_THREADS 2

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int condition_met = 0;

void* thread_func(void* arg) {
    long id = (long)arg;
    printf("Thread %ld starting.\n", id);
    pthread_mutex_lock(&mutex);
    while (!condition_met) {
        pthread_cond_wait(&cond, &mutex);
    }
    pthread_mutex_unlock(&mutex);

    printf("Thread %ld proceeding.\n", id);
    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];
    
   
    pthread_cond_signal(&cond); // First signal
    pthread_cond_signal(&cond); // Second signal
    

    // Create threads after signals
    for(long i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, thread_func, (void*)i);
    }
    
    // Join threads
    for(int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);

    return 0;
}
