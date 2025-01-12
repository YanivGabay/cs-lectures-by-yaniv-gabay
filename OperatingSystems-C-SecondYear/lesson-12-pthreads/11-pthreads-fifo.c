// File: pthread_fifo_example.c
// Compile with: gcc -Wall -pthread pthread_fifo_example.c -o pthread_fifo_example
//
// Overview:
// This example demonstrates how to combine pthreads with a named pipe (FIFO).
// The main thread creates the FIFO (if it does not already exist) and spawns two threads:
//   - A writer thread that writes a message to the FIFO every second.
//   - A reader thread that continuously reads messages from the FIFO and prints them.
// The main thread then calls pthread_exit() so that it finishes while the worker threads continue.
// (Note: In a real-world situation, proper synchronization and error handling should be used.)

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include <time.h>

// Name of the FIFO to be used for IPC
#define FIFO_NAME "mypipe"

// Message buffer size
#define BUFSIZE 256

// Number of messages the writer thread sends
#define NUM_MESSAGES 10

// Function prototypes for our threads
void* writer_thread(void *arg);
void* reader_thread(void *arg);

int main() {
    pthread_t writer, reader;
    int status;
    
    // Create the named pipe (FIFO) if it does not exist.
    // Permissions: 0666 (readable and writable by everyone)
    if (mkfifo(FIFO_NAME, 0666) == -1) {
        if (errno != EEXIST) {
            perror("mkfifo failed");
            exit(EXIT_FAILURE);
        }
    }
    
    printf("Main: FIFO '%s' created or already exists.\n", FIFO_NAME);
    
    // Seed the random number generator for the writer thread.
    srand((unsigned) time(NULL));
    
    // Create the writer thread.
    status = pthread_create(&writer, NULL, writer_thread, NULL);
    if (status != 0) {
        fprintf(stderr, "pthread_create (writer) failed\n");
        exit(EXIT_FAILURE);
    }
    
    // Create the reader thread.
    status = pthread_create(&reader, NULL, reader_thread, NULL);
    if (status != 0) {
        fprintf(stderr, "pthread_create (reader) failed\n");
        exit(EXIT_FAILURE);
    }
    
    // Main thread exits here (using pthread_exit so that the process lives on for the threads)
    printf("Main thread: Exiting while threads continue to run.\n");
    pthread_exit(NULL);
    
    // Note: Code after pthread_exit() is never executed.
    return EXIT_SUCCESS;
}

// The writer_thread function writes messages to the FIFO.
void* writer_thread(void *arg) {
    int fd;
    char message[BUFSIZE];
    
    // Open the FIFO for writing. (Open in blocking mode)
    fd = open(FIFO_NAME, O_WRONLY);
    if (fd < 0) {
        perror("writer_thread: open FIFO for writing failed");
        pthread_exit(NULL);
    }
    
    for (int i = 0; i < NUM_MESSAGES; i++) {
        // Compose a message
        snprintf(message, BUFSIZE, "Message %d from writer (pid: %d)", i, getpid());
        printf("Writer thread: writing: \"%s\"\n", message);
        
        // Write message to FIFO
        if (write(fd, message, strlen(message) + 1) < 0) {
            perror("writer_thread: write failed");
            close(fd);
            pthread_exit(NULL);
        }
        
        sleep(1); // Wait one second before sending the next message
    }
    
    close(fd);
    pthread_exit(NULL);
}

// The reader_thread function reads messages from the FIFO and prints them.
void* reader_thread(void *arg) {
    int fd;
    char buf[BUFSIZE];
    int nbytes;
    
    // Open the FIFO for reading. (Blocking read)
    fd = open(FIFO_NAME, O_RDONLY);
    if (fd < 0) {
        perror("reader_thread: open FIFO for reading failed");
        pthread_exit(NULL);
    }
    
    // Continuously read messages.
    // This example will exit when the writer closes its end,
    // making read return 0.
    while ((nbytes = read(fd, buf, BUFSIZE)) > 0) {
        buf[nbytes] = '\0'; // Ensure null termination
        printf("Reader thread: received: \"%s\"\n", buf);
    }
    
    if (nbytes < 0) {
        perror("reader_thread: read failed");
    } else if (nbytes == 0) {
        printf("Reader thread: writer has closed the FIFO.\n");
    }
    
    close(fd);
    pthread_exit(NULL);
}
