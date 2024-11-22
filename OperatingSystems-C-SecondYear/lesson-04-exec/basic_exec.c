#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>



int main() {
    const int EXAMPLE = 1;

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(1);
    }

    if (pid == 0)
    {
        printf("Child process going into the exec\n");
        int status;
        switch (EXAMPLE)
        {
        case 1:
        //with execl we can pass arguments as a sequence of strings
            status = execl("/bin/ls", "ls", "-l", NULL);
            break;
        case 2:
            //with execv we can pass a char array of arguments
            char *args[] = {"/bin/ls", "-l", NULL};
            status = execv("/bin/ls", args);
            break;
      
        default:
            break;
        }
        if (status == -1) {
            perror("execl");
            exit(1);
        }
        
    }
    else {
        printf("Parent process waiting for the child\n");
        wait(NULL);
        printf("Parent process finished\n");
    }

    return 0;
}