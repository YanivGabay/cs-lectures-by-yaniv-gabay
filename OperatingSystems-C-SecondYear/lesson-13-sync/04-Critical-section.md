## Critical section
- A critical section is a section of code that is executed by multiple threads and where the sequence of execution must be controlled.

Basiclly, there is a section of our code, which a single thread can execute at a time. This is called a critical section.


### Example
```c
#include <stdio.h>
#include <pthread.h>

int x = 0;

void* thread_function(void* arg) {
    for (int i = 0; i < 1000000; i++) {
        x++;
    }
    return NULL;
}

int main() {
    pthread_t thread1, thread2;
    pthread_create(&thread1, NULL, thread_function, NULL);
    pthread_create(&thread2, NULL, thread_function, NULL);
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    printf("x = %d\n", x);
    return 0;
}
```
In this example, we have two threads that increment a shared variable `x` 1,000,000 times each. The expected final value of `x` is 2,000,000. However, because both threads are accessing `x` concurrently, the final value of `x` is unpredictable.

What is the critical section in this example?

The critical section is the code block where `x` is being incremented (`x++`). This is the section of code that must be executed by only one thread at a time.

### What we want to achieve?

- Mutual Exclusion: Only one thread can execute the critical section at a time.
- Progress: If no thread is executing the critical section and some threads want to execute it, in some X time a thread will enter the critical section. there will be no Freezing.
important notice - A thread that dont want to enter the critical section, will not prevent other threads from entering the critical section.
- Bounded Waiting: If a thread wants to enter the critical section, it will eventually enter it. There will be no starvation.
Example of bounded waiting: 
lets say we have 20 threads, 1 is in the critical section, and 19 are waiting,  lets call the 1 A, and the 19 B1, B2, B3, ... B19.
after A finishes, one of the threads B1, B2, B3, ... B19 will enter the critical section, lets say B1.
and A will be in the Waiting queue, and the other threads will be in the Waiting queue.
We want to make sure, that B1 will not enter the critical section again, before all the other threads will enter the critical section at least once.
or just in general, that there wont be a situation where threads are "lost" in the waiting queue.


### How to achieve this?
#### Protocol A
we will use an example using only 2 threads, and an atomic turn variable.
```c
#include <stdio.h>
#include <pthread.h>

int x = 0;
atomic int turn = 0;

//this is P0 code
while (true) {
    while (turn != 0);
    // critical section
    x++;
    turn = 1;
    // end of critical section
}
```

//this is P1 code
```c
while (true) {
    while (turn != 1);
    // critical section
    x++;
    turn = 0;
    // end of critical section
}
```

we achive mutual exclusion, because only one thread can enter the critical section at a time.
Progression is a bit limited, cus until p1 will enter his critical section, finishes, than p0 will enter his critical section, finishes, and than p1 will enter his critical section again, and so on.
Bounded waiting is achieved, because if p0 wants to enter the critical section, and p1 is in the critical section, p0 will wait until p1 finishes, and than p0 will enter the critical section.

### What is the problem with this solution?
- Busy waiting: The threads are continuously checking the `turn` variable in a loop, which consumes CPU resources and is inefficient.
- We do a "schedule" , no matter if the other thread wants to enter the critical section or not, he gets the turn.

#### Protocol B
two threads example with a atomic bool array[2] = {false,false}

```c
#include <stdio.h>
#include <pthread.h>

int x = 0;
atomic bool flag[2] = {false, false};

//this is P0 code
while (true) {
    flag[0] = true;
    while (flag[1]);
    // critical section
    x++;
    flag[0] = false;
    // end of critical section
}
```

//this is P1 code
```c
while (true) {
    flag[1] = true;
    while (flag[0]);
    // critical section
    x++;
    flag[1] = false;
    // end of critical section
}
```

we achive mutual exclusion, because only one thread can enter the critical section at a time.
- Progression can have a deadlock situation, if both threads immediately after the flag[0] = true, and flag[1] = true, and than both threads will wait for the other thread to finish, and than both threads will wait forever.


#### Lets try Protocol C
we will use both the bool array,and the turn variable.

```c
#include <stdio.h>
#include <pthread.h>

int x = 0;
atomic bool flag[2] = {false, false};
atomic int turn = 0;

//this is P0 code
while (true) {
    flag[0] = true;//I want to enter the critical section
    turn = 1; // i allow the other thread to enter the critical section
    while (flag[1] && turn == 1); // i wait here, while its his turn AND he wants to enter the critical section
    // critical section
    x++;
    flag[0] = false;
    // end of critical section
}
```

//this is P1 code
```c
while (true) {
    flag[1] = true;//I want to enter the critical section
    turn = 0; // i allow the other thread to enter the critical section
    while (flag[0] && turn == 0); // i wait here, while its his turn AND he wants to enter the critical section
    // critical section
    x++;
    flag[1] = false;
    // end of critical section
}
```

we achive mutual exclusion, because only one thread can enter the critical section at a time.
Progression is achieved, because if p0 wants to enter the critical section, and p1 is in the critical section, p0 will wait until p1 finishes, and than p0 will enter the critical section.
We wont have any deadlocks, cus now we wait for both variables, and we allow the other thread to enter the critical section, so we wont have a situation where both threads are waiting for each other.
Bound waiting is achieved, because if p0 wants to enter the critical section, and p1 is in the critical section, p0 will wait until p1 finishes, and than p0 will enter the critical section.

Remember, that its only two threads solutions, we very often want much more than 2 threads, so this isnt a good solution for us.


