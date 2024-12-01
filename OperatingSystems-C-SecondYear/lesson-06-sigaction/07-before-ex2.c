#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>


int global_flag = 1;

void handle_sigalrm(int signum) {
    //for sigalrm
    printf("Caught SIGALRM (signal number %d). This handler will reset to default after this.\n", signum);
    global_flag = 0;
}

int main() {
    const int seconds = 5;
    struct sigaction sa;

  
    sa.sa_handler = handle_sigalrm;
    sa.sa_flags = 0; 
    sigemptyset(&sa.sa_mask);   // No additional signals blocked

    if (sigaction(SIGALRM, &sa, NULL) == -1) {
        perror("Error: sigaction for SIGALRM failed");
        exit(EXIT_FAILURE);
    }

    printf("Process PID: %d\n", getpid());
  
    int bigest = 0;
    for (int i = 0; i < 10; i++) {
       
        int counter = 0;
        while(global_flag)
        {
            alarm(seconds);
            //take input from the user
            printf("Enter a number: ");
            int number;
            scanf("%d", &number);
            counter++;

        }
        if(counter>bigest)
        {
            bigest = counter;
        }

    }

    return 0;
}
