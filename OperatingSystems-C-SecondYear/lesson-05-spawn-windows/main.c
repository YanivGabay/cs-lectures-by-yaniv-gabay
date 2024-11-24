#include <stdio.h>
#include <stdlib.h>
#include <process.h> // For spawn functions

int main()
{
    const int EXAMPLE = 2; // Choose which spawn variant to use
    //example 1 is the only working as wanted example
    printf("Parent process running...\n");

    if (EXAMPLE == 1)
    {
        // Example 1: Using _spawnl
        printf("Launching calculator using _spawnl...\n");
        int status = spawnl(_P_WAIT, "./calculator.exe", "calculator", "5", "+", "3", NULL);
        // the are spawn flags:
        //  _P_WAIT: Wait for the spawned process to finish
        //  _P_NOWAIT: Don't wait for the spawned process to finish
        //  _P_DETACH: Detach the spawned process, basicly making it independent from the parent process, without any connection
        if (status == -1)
        {
            perror("spawnl failed");
        }
        else
        {
            printf("Calculator finished with status: %d\n", status);
        }
    }
    else if (EXAMPLE == 2)
    {
        // Example 2: Using spawnv
        printf("Launching calculator using _spawnv...\n");
        //char *args[] = {"calculator", "8", "*", "4", NULL};
        char *args[] = {"calculator.exe", "8", "X", "4", NULL};
        ///////////////// IMPORTANT NOTE //////////////////
        //the problem is, when we pass * as an argv to the child process, it will be interpreted as a wildcard
        // which means, it will be replaced by the shell with all the files in the current directory
        // so i had this BUG for two days, and didnt understand why is the child process got like 8 arguments
        //so the fix is just to pass a X instead of a * (or any other token than *), and it will work
        // we can also use //* instead of * to escape the wildcard
        // or use #define _CRT_glob 0 to disable the wildcard expansion


        printf("Arguments being passed to _spawnv:\n");
        for (int i = 0; args[i] != NULL; i++)
        {
            printf("args[%d]: %s\n", i, args[i]);
        }
        // SPAWNV inject environment variables to the child process
        // now we have explicity disabled the environment variables
        // which means, we cant see the child output, because we didnt pass the environment variables
        // which one of them is stdout
        // so the best solution is just to use spawnl
        int status = spawnv(_P_WAIT, "./calculator.exe", args);
        //testinbg spawnvp
       // int status = spawnvp(_P_WAIT, "./calculator.exe", args);
       
        if (status == -1)
        {
            perror("spawnv failed");
        }
        else
        {
            printf("Calculator finished with status: %d\n", status);
        }
    }
    else if (EXAMPLE == 3)
    {
        //THIS EXAMPLE CURRENTLY FAILS

        // in order to fix example 2, to stop injecting , we can use spawnv
        // but we pass an empty environment array, so still we wont be able to see the child output
        printf("Launching calculator using _spawnve with custom environment...\n");

        // Correct args array
        char *args[] = {"calculator.exe", "8", "*", "4", NULL};

        // Minimal environment
        char *env[] = {NULL}; // No inherited environment variables
        
        
       

        // Debug: Print arguments being passed to _spawnve
        printf("Arguments being passed to _spawnve:\n");
        for (int i = 0; args[i] != NULL; i++)
        {
            printf("args[%d]: %s\n", i, args[i]);
        }

        // Launch the child process with a minimal environment
        int status = spawnve(_P_WAIT, "./calculator.exe", args, env);
        if (status == -1)
        {
            printf("spawnve failed\n");
            perror("spawnve failed");
        }
        else
        {
            printf("Calculator finished with status: %d\n", status);
        }
        printf("Parent continues without waiting. Child launched with status: %d\n", status);
    }
    else
    {
        fprintf(stderr, "Invalid example selected\n");
    }

    return 0;
}
