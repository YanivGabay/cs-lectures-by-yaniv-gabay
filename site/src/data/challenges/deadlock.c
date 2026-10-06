/*
 * Challenge (Lesson 14): detect the deadlock
 *
 * Two threads take the same two mutexes in OPPOSITE order — the dining philosophers'
 * problem with two philosophers. Each grabs one lock and waits forever for the other.
 * Run it: it hangs (the site stops it after 10 seconds).
 *
 * Your task: thread B must not wait forever. Replace its second pthread_mutex_lock()
 * with pthread_mutex_timedlock() and a 2-second deadline. If it times out (ETIMEDOUT),
 * print "deadlock detected", release what it holds, and return.
 * Done when the program prints "deadlock detected" and exits on its own.
 */
#include <stdio.h>
#include <errno.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t fork_left = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t fork_right = PTHREAD_MUTEX_INITIALIZER;

void *philosopher_a(void *arg)
{
    pthread_mutex_lock(&fork_left);
    printf("[A] has the left fork, reaching for the right...\n");
    fflush(stdout);
    sleep(1);
    pthread_mutex_lock(&fork_right);   // waits for B
    printf("[A] eating\n");
    pthread_mutex_unlock(&fork_right);
    pthread_mutex_unlock(&fork_left);
    return NULL;
}

void *philosopher_b(void *arg)
{
    pthread_mutex_lock(&fork_right);
    printf("[B] has the right fork, reaching for the left...\n");
    fflush(stdout);
    sleep(1);

    // TODO: wait at most 2 seconds:
    //   struct timespec deadline; clock_gettime(CLOCK_REALTIME, &deadline); deadline.tv_sec += 2;
    //   if (pthread_mutex_timedlock(&fork_left, &deadline) == ETIMEDOUT) { ... }
    pthread_mutex_lock(&fork_left);    // waits for A — forever

    printf("[B] eating\n");
    pthread_mutex_unlock(&fork_left);
    pthread_mutex_unlock(&fork_right);
    return NULL;
}

int main(void)
{
    pthread_t a, b;
    pthread_create(&a, NULL, philosopher_a, NULL);
    pthread_create(&b, NULL, philosopher_b, NULL);
    pthread_join(b, NULL);
    pthread_join(a, NULL);
    printf("both philosophers finished\n");
    return 0;
}
