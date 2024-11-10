//
/*

https://man7.org/linux/man-pages/index.html

man7 is your BIGGEST friend for all system call posix information
you can search for any system call and get all the information you need
any include functions, the whole course is there and much much much more


*/









#include <fcntl.h>
// Example: Using open() to open a file
int fd = open("example.txt", O_RDONLY);
if (fd == -1) {
    perror("open");
    exit(EXIT_FAILURE);
}

#include <unistd.h>
// Example: Using fork() to create a new process
pid_t pid = fork();
if (pid == -1) {
    perror("fork");
    exit(EXIT_FAILURE);
} else if (pid == 0) {
    // Child process
    printf("This is the child process\n");
    return 0;
} else {
    // Parent process
    printf("This is the parent process\n");
    return 0;
}

// wait for childrens
#include <sys/wait.h>
// Example: Using wait() to wait for a child process
int status;
pid_t child_pid = wait(&status);

#include <errno.h>

//MORE COMMON ON OUR COURSE 
// Example: Using perror() to print an error message
int result = remove("nonexistent_file.txt");
if (result == -1) {
    perror("error removing file");
}
//^^^^^^^^^ MORE COMMON ON OUR COURSE ^^^^^^^^^^^^^^^


// Example: Using errno to check for errors
FILE *file = fopen("nonexistent.txt", "r");
if (file == NULL) {
    printf("Error opening file: %s\n", strerror(errno));
}


#include <stdio.h>
// Example: Using printf() to print a message
printf("Hello, World!\n");

#include <stdlib.h>
// Example: Using malloc() to allocate memory
int *arr = (int *)malloc(10 * sizeof(int));
if (arr == NULL) {
    perror("malloc");
    exit(EXIT_FAILURE);
}

#include <sys/types.h>
// Example: Using pid_t type for process IDs
// this includes brigns the actuall type of: pid_t
// which is a type that is used to represent process IDs (so its some kind of integer)


 #include <unistd.h>
 // the actual getpid() function and getppid() function
 // comes from this include

// Each code we run has a process ID (PID)
// the operating system allocates to your code a process, which has a unique PID
pid_t my_pid = getpid();
printf("My process ID is %d\n", my_pid);

pid_t parent_pid = getppid();
//this function returns the parent process ID
//we will learn more of this later
