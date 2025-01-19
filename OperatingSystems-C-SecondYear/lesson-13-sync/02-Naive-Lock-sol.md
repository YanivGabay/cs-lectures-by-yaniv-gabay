## Naive solution - Lock

### Concept
we will implement a naive solution to the lock problem. We will use a global variable to indicate whether the lock is available or not. If the lock is available, we will acquire the lock and set the global variable to indicate that the lock is not available. If the lock is not available, we will wait until the lock is available.

### Implementation
```c
#include <stdio.h>
#include <pthread.h>

int lock = 1;

void acquire_lock() {
    while (lock == 0);
    lock = 0;
}

void release_lock() {
    lock = 1;
}

void* thread_function(void* arg) {
    acquire_lock();
    printf("Thread %d acquired the lock\n", (int)arg);
    release_lock();
    printf("Thread %d released the lock\n", (int)arg);
    return NULL;
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

### Explanation
* We have a global variable `lock` that indicates whether the lock is available or not. If `lock` is 1, the lock is available. If `lock` is 0, the lock is not available.
* The `acquire_lock` function is used to acquire the lock. It waits in a loop until the lock is available (i.e., `lock` is 1) and then sets `lock` to 0 to indicate that the lock is not available.
* The `release_lock` function is used to release the lock. It simply sets `lock` to 1 to indicate that the lock is available.
* In the `thread_function`, each thread acquires the lock, prints a message indicating that it has acquired the lock, releases the lock, and prints a message indicating that it has released the lock.
* In the `main` function, we create two threads that call the `thread_function` with different arguments. Each thread acquires and releases the lock, and the output shows that the threads are taking turns acquiring and releasing the lock.

### What is the problem with this solution?
if both threads are running at the same time, they can both check the lock variable and see that it is available (i.e., `lock` is 1). Then, both threads can acquire the lock and set `lock` to 0, which means that both threads think they have acquired the lock. 
that means that two threads can acquire the lock at the same time, which is not the intended behavior of a lock.