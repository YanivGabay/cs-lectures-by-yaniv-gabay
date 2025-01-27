#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>



//very intresting thing called
// Spurious wakeups
//https://en.wikipedia.org/wiki/Spurious_wakeup

#define NUM_THREADS 20

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int condition_met = 0;

void* thread_func(void* arg) {
    long id = (long)arg;

    pthread_mutex_lock(&mutex);
    while (!condition_met) {
        //we put the loop here to avoid spurious wakeups
        // if for some case a thread wakes up without
        // the condition being met
        // it will go back to sleep via cond_wait
        pthread_cond_wait(&cond, &mutex);
    }
    pthread_mutex_unlock(&mutex);

    printf("Thread %ld proceeding.\n", id);
    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];
    
    // Create threads
    for(long i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, thread_func, (void*)i);
    }
    
    sleep(2); // Simulate some work
    
    // Wake up all threads one by one using pthread_cond_signal
    pthread_mutex_lock(&mutex);
    condition_met = 1;
    pthread_mutex_unlock(&mutex);
    
    for(int i = 0; i < NUM_THREADS; i++) {
        pthread_cond_signal(&cond);
        usleep(100000); // Small delay to observe sequential waking
    }
    
    // Join threads
    for(int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);

    return 0;
}
