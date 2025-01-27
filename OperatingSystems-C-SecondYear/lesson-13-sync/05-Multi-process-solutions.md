## Multi Process Solutions


### The Bakery Algorithm
Guess where the name comes from? Hint: Bakeries.........


#### Concept

- Each Thread has a UNIQUE id from 0 to n-1.
- The Algorithm can run on a system with multiple processors.
- (a,b) < (c,d) if a < c or (a=c and b < d) - standard lexicographic order.
- max(a0,a1...an-1) return the biggest or equal than all other ai.

#### Implementation

```c
#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>

#define N 20
bool choosing[N] = {false}//if currently im "getting a number to wait in line"

int number[N] = {0}; // queue number actuall my place in line

//the generel algorithm before we show threads

```c
while(1)
{
    choosing[me] = true // i want a number in line
    number[me] = max(number[0],number[1]...number[n-1]) + 1; // the max number in line + 1
    // pay attention, that the actual number might not be unique, but the order is unique. (each me is unique)
    choosing[me] = false;

    for (int other = 0; other < N; other++)
    {
        while(choosing[other]); // wait until the other thread gets a number
        while((number[other] != 0) && (number[other],other) < (number[me],me)); // wait until the other thread gets a number
        // you didnt know we can compare lexographically in C, right? :)

        //critical section
        //do something
        number[me] = 0;
        //end of critical section
    }
}

```

//lets show the code for the threads

```c
void* thread_function(void* arg)
{
    int me = (int)arg;
    while(1)
    {
        choosing[me] = true // i want a number in line
        number[me] = max(number[0],number[1]...number[n-1]) + 1; // the max number in line + 1
        // pay attention, that the actual number might not be unique, but the order is unique. (each me is unique)
        choosing[me] = false;

        for (int other = 0; other < N; other++)
        {
            while(choosing[other]); // wait until the other thread gets a number
            while((number[other] != 0) && (number[other],other) < (number[me],me)); // wait until the other thread gets a number
            // you didnt know we can compare lexographically in C, right? :)

           
        }
        //only after we check with all other threads, we can enter the critical section
         //critical section
         //do something
        number[me] = 0;
        //end of critical section
        //than we go back to choosing our number
    }
    return NULL;
}

int main()
{
    pthread_t threads[N];
    for (int i = 0; i < N; i++)
    {
        pthread_create(&threads[i], NULL, thread_function, (void*)i);
    }
    for (int i = 0; i < N; i++)
    {
        pthread_join(threads[i], NULL);
    }
    return 0;
}
```

#### Explanation
Just imagine you are in a bakery, and you want to get a number to wait in line.
You get a number, and you wait until your number is called.
If you are the first in line, you can go to the cashier and buy your bread.
If you are not the first in line, you wait until your number is called.
What is waiting until your number is called?,while other threads get their number, and all threads got their number, and their number is smaller than yours.
Then you can go to the cashier and buy your bread.

#### Second Solution
Here we assume, we can block system interrupts using our code, that means that we can stop the system from running other threads in a checking situation or lock, and we can run our code without any interruptions.

```c
enum LOCK {FREE, BUSY};

lock_acquire(LOCK *lock)
{
    disable_interrupts(); // some imaginary function that stops the system from running other threads
    while (*lock == BUSY){
        enable_interrupts(); // some imaginary function that allows the system to run other threads
        //we can enable interrupts here, because we are not in the critical section
        disable_interrupts(); // some imaginary function that stops the system from running other threads
        //back to disabled before we check the lock again
    }

    *lock = BUSY;
    //critical section
    //do something
    enable_interrupts(); // some imaginary function that allows the system to run other threads
}

lock_release(LOCK *lock)
{
    *lock = FREE;
}
```