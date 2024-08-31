#include <stdio.h>
#include <unistd.h> // for fork()

// Carefull, this code should run on a Linux machine
// Windows does not support fork() system call
// fork() creates a new process by duplicating the existing process
// its a system call that is used to create processes
// which means it creates a new process which runs concurrently with the calling process

// some basic info about fork return value:
/*
Explanation:

fork() returns a value:
Negative Value: Fork failed.
Zero (0): This is the child process.
Positive Value: This is the parent process, and the value returned is the PID of the child
PID = Process ID
*/

// small tip , your new friend for all system call information is :
// https://man7.org/linux/man-pages/index.html
// you can search for any system call and get all the information you need


void check_valid_process(pid_t status);

int main()
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