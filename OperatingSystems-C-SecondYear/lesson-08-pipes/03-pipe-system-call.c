#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/*

Understanding Pipe File Descriptors After fork
When you create a pipe using pipe(pipefd), it generates two file descriptors:

pipefd[0]: The read end of the pipe.
pipefd[1]: The write end of the pipe.
After calling fork(), both the parent and child processes inherit copies of these file descriptors. This means:

Parent Process:
Has access to both pipefd[0] (read) and pipefd[1] (write).
Child Process:
Also has access to both pipefd[0] (read) and pipefd[1] (write).
However, for effective IPC, it's essential to close the unused ends in each process to prevent unintended behavior, such as deadlocks or resource leaks.

so the situation can be visualized as follows:

Before fork():
Parent Process:
- pipefd[0] (read end)
- pipefd[1] (write end)

After fork():
Parent Process:
- pipefd[0] (closed)
- pipefd[1] (write end, used to write data)

Child Process:
- pipefd[0] (read end, used to read data)
- pipefd[1] (closed)

Tips:
- Always check the return values of system calls (pipe, fork, read, write, etc.) and handle errors appropriately.
- Always check the return values of read and write to handle cases where not all data is read or written in one operation.
- Always Close Unused Ends: After creating a pipe, close the unused ends in the parent and child processes to avoid issues like deadlocks.
- Use sleep() or other synchronization mechanisms to ensure that the child process is ready to read before the parent writes data to the pipe.
- Of course : Use waitpid in the parent process to wait for child processes to prevent zombie processes.
- EOF Detection: When the parent closes pipefd[1] after writing, the child’s read operation can detect EOF, allowing it to know that no more data will be sent.
- Basicly while read is > 0, the child will keep reading from the pipe, when it reaches EOF (recived 0) it will stop.


But how does it actually works?

a. Writing Data:
Parent Writes to pipefd[1]:
The write system call in the parent writes data to the write end of the pipe.
This data is placed into a kernel-managed buffer associated with the pipe.
The data remains in the buffer until it is read by the child process.
if the buffer is full, the write operation will block until space is available in the buffer (probably the child is not reading from the pipe).
but its unlikely to happen, because the buffer size is 65536 bytes (64KB) in linux.

b. Reading Data
Child Reads from pipefd[0]:
The read system call in the child reads data from the read end of the pipe.
The data read is the same data that the parent wrote to pipefd[1].
If there is no data available to read, the read operation will block until data is written to the pipe.
When the parent closes pipefd[1], the child’s read operation will return 0, indicating EOF (end of file).

*/

int main() {
    int pipefd[2];
    pid_t pid;
    char buffer[100];

    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid = fork();
    if (pid == 0) { // Child process
        close(pipefd[1]); // Close unused write end
        ssize_t bytesRead = read(pipefd[0], buffer, sizeof(buffer)); // Read from the pipe
        if (bytesRead == -1) {
            perror("read");
            exit(EXIT_FAILURE);
        }
        printf("Child received: %s\n", buffer);
        printf("str length of buffer: %ld\n", strlen(buffer));
        close(pipefd[0]);
        exit(EXIT_SUCCESS);
    } else { // Parent process
        close(pipefd[0]); // Close unused read end
        sleep(2);
        ssize_t bytesWritten = write(pipefd[1], "Hello, Pipe!", strlen("Hello, Pipe!") + 1); 
        if (bytesWritten == -1) {
            perror("write");
            exit(EXIT_FAILURE);
        }
        close(pipefd[1]);
    }

    return 0;
}
