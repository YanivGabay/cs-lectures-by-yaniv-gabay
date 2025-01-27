


### What are conditional waiting?

its a sync mechanism, used in multi thread programming.
it allows threads to wait for certian condition to be met
before proceeding.

this enable the thread to:
- wait for a condition to be met
- signal or broadcast other threads that the condition has been met

### How does it works?

- mutex , protects the condition variable
- condition variable,
  used to block a thread until a condition is met


So how does it works:

//condition_met is a bool global variable
// whitch we change on a critical section


```c
bool condition_met

void *thread_init(void *arg) {
    pthread_mutex_lock(&mutex);
    //do some init work
    //dynamic allocation

    // change the condition
    condition_met = true;

    // signal other threads that the condition has been met
    // "awaken" only ONE thread from the waiting list
    pthread_cond_signal(&cond);
    //we can broadcast to all threads
    pthread_cond_broadcast(&cond);
    
    //LOOK BELOW we use the same mutex as the below.

    pthread_mutex_unlock(&mutex);

void *thread_function(void *arg) {
    pthread_mutex_lock(&mutex);

    while (!condition_met) {
      pthread_cond_wait(&cond, &mutex);
      //WE PASS THE MUTEX WE USED TO 
      //LOCK THE CONDITION
    //IMPORTANT:
     // if the condition is not met,
    // the thread will be blocked
    // and the mutex will be released
    // when the condition is met,
     //the thread will be unblocked
     //and the mutex will be re-acquired
      
    }

    pthread_mutex_unlock(&mutex);
}
```

### how to notify other threads 
that the condition has been met?

two ways:
- pthread_cond_signal(&cond);
this will wake up one thread that is waiting on the
 condition variable

- pthread_cond_broadcast(&cond);
this will wake up all threads that are waiting on the
 condition variable

*/


///// IMPORTANT
/*
If we try to awake any threads
but we dont have any sleeping threads
the signal will be lost!!!

*/
