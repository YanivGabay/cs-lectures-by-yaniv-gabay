## Synchronization Hardware

### Description
The idea is pretty simple, the hardware will help us achieve the wanted results.
How? be giving us atomic operations, that will help us achieve mutual exclusion, bounded waiting, and progression.

Most common are Test and Set,Swap ,Compare and Swap (the rest you didnt learn like Fetch and Add, Load Linked, Store Conditional, etc).

### Test and Set
Test and Set is a simple atomic operation that sets a variable to a value and returns the old value of the variable.
```c
//Yoram has mistake in his book, there arent & in the function signature in C.
bool TestAndSet(bool* target) {
    bool old = *target;
    *target = true;
    return old;
}
```
WE ASSUME THIS IS AN ATOMIC OPERATION, MEANING NO OTHER THREAD CAN INTERFERE WITH THIS OPERATION.

#### Example
```c
#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>

bool lock = false;

bool TestAndSet(bool* target) {
    bool old = *target;
    *target = true;
    return old;
}

void acquire_lock() {
    while (TestAndSet(&lock));
}

void release_lock() {
    lock = false;
}

void* thread_function(void* arg) {
    while(1)
    {
    acquire_lock();
    // critical section
    printf("Thread %d acquired the lock\n", (int)arg);
    // end of critical section
    release_lock();
    printf("Thread %d released the lock\n", (int)arg);
  
    }
   
}

int main() {
    pthread_t thread1, thread2;
    pthread_create(&thread1, NULL, thread_function, (void*)1);
    pthread_create(&thread2, NULL, thread_function, (void*)2);
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    return 0;
}
```

### Problems with the above solution
1. Starvation - can happen, why? because the lock is not fair, and the lock can be acquired by the same thread over and over again.

### Swap
Swap is a simple atomic operation that swaps the values of two variables and returns the old value of one of the variables.
```c
//again in Yoram book there are & but there shouldnt be.
void Swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}
```

#### Example
```c
#include <stdio.h>
#include <pthread.h>

bool lock = false;

void Swap(bool* a, bool* b) {
    bool temp = *a;
    *a = *b;
    *b = temp;
}

void thread_function(void* arg) {
    bool key = false;
    while(1)
    {
        key = true; // i want to  lock
        while(key == true) // wait until the other thread gets the lock
        {
            Swap(&lock,&key);//atomic operation
        }
        // critical section
        printf("Thread %d acquired the lock\n", (int)arg);
        // end of critical section
        lock = false;
        printf("Thread %d released the lock\n", (int)arg);
    }
}
```

### Protocols using the above operations
As we said, both of them dont solve the starvation problem, but they can be used to implement protocols that will solve the starvation problem.
We will use the following variables:
```c
bool waiting[N] = {false}; // who wants to enter the critical section, the one whos exit, will invite the next one - fairness
bool lock = false; // the lock, when it false, you can enter the critical section

//each thread will have a local variable key, that will indicate if the thread wants to enter the critical section.
```

#### Protocol A
```c
#include <stdio.h>
#include <pthread.h>

#define N 10
bool waiting[N] = {false};
bool lock = false;

bool TestAndSet(bool* target) {
    bool old = *target;
    *target = true;
    return old;
}

void thread_function(void* arg) {
    int me = (int)arg;
    bool key = false;
    while(1)
    {
       waiting[me] = true;
       key = true;
       while(waiting[me] && key)
       {
           key = TestAndSet(&lock);
       }
         waiting[me] = false;
        // critical section
        printf("Thread %d acquired the lock\n", me);
        // end of critical section
        int other = (me + 1) % N;
        while((other != me) && !waiting[other])
        {
            other = (other + 1) % N;//find the next thread that wants to enter the critical section
        }
        if(other != me)
        {
            waiting[other] = false;//tell the other thread to enter the critical section
        }
        else
        {
            lock = false;//no other thread wants to enter the critical section, so release the lock
        }
    }
}
```