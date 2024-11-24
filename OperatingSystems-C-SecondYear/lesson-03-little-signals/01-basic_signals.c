#include <sys/types.h>
#include <signal.h>
#include <stdio.h>

#include <stdlib.h>


void catch_int(int sig_num);

int main() {
    signal(SIGINT, catch_int);// SIGINT is the signal that is sent when you press Ctrl+C
    
    while (1) {
        printf("Hello, World!\n");
        _sleep(1);//for windows _sleep(1) for linux sleep(1)
    }

    return 0;
}

void catch_int(int sig_num) {
   printf("Caught signal %d\n", sig_num);
    exit(0); 
 
}