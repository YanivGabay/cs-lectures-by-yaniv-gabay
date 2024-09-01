#include <stdio.h>
#include <unistd.h> // for fork()




// Carefull, this code should run on a Linux machine


// now lets say, we want to run this same code, but we want to run it 5 times

// a common mistake would be to just add a for loop, and run the code 5 times

void check_valid_process(pid_t status);


// lets think together, what would happen if we run this code 5 times?
// what would be the output?
// why is that?
// how can we fix it?
int main()
{

    for (int i = 0; i < 5; i++)
    {
        pid_t pid = fork(); // fork() creates a new process

        if (pid < 0)
        {
            // I RECOMMEND TO CREATE A FUNCTION FOR ALL FUTURE ERROR HANDLING
            // Fork failed
            perror("fork failed");
            return 1;
        }
        else if (pid == 0)
        {
            // Child process
            printf("This is the child process. PID: %d\n", getpid());
        }
        else
        {
            // Parent process
            printf("This is the parent process. PID: %d, Child PID: %d\n", getpid(), pid);
        }
    }

  

    return 0;
}

// example check if the process is valid,else we exit the program
void check_valid_process(pid_t status)
{
    if (status < 0)
    {
        perror("Failed To open procceses");
        exit(EXIT_FAILURE);
    }
}