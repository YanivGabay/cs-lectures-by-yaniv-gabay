#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>

// FIFO names
#define WRITER_TO_READER_FIFO "writer_to_reader_fifo"
#define READER_TO_WRITER_FIFO "reader_to_writer_fifo"

// Buffer size
#define BUFSIZE 256

// Number of messages
#define NUM_MESSAGES 10

void* writer_thread(void* arg);
void* reader_thread(void* arg);

int main() {
    pthread_t writer, reader;

    // Create FIFOs
    if (mkfifo(WRITER_TO_READER_FIFO, 0666) == -1) perror("mkfifo writer_to_reader");
    if (mkfifo(READER_TO_WRITER_FIFO, 0666) == -1) perror("mkfifo reader_to_writer");

    // Create threads
    if (pthread_create(&writer, NULL, writer_thread, NULL) != 0) {
        perror("Failed to create writer thread");
        exit(EXIT_FAILURE);
    }
    if (pthread_create(&reader, NULL, reader_thread, NULL) != 0) {
        perror("Failed to create reader thread");
        exit(EXIT_FAILURE);
    }

    // Wait for threads to finish
    pthread_join(writer, NULL);
    pthread_join(reader, NULL);

    // Remove FIFOs after use
    unlink(WRITER_TO_READER_FIFO);
    unlink(READER_TO_WRITER_FIFO);

    return 0;
}

void* writer_thread(void* arg) {
    int write_fd = open(WRITER_TO_READER_FIFO, O_WRONLY);
    int read_fd = open(READER_TO_WRITER_FIFO, O_RDONLY);
    char message[BUFSIZE], response[BUFSIZE];

    if (write_fd == -1 || read_fd == -1) {
        perror("writer_thread: Failed to open FIFOs");
        pthread_exit(NULL);
    }

    for (int i = 0; i < NUM_MESSAGES; i++) {
        // Write a message to the reader
        snprintf(message, BUFSIZE, "Message %d from writer", i);
        if (write(write_fd, message, strlen(message) + 1) == -1) {
            perror("writer_thread: Failed to write message");
            close(write_fd);
            close(read_fd);
            pthread_exit(NULL);
        }
        printf("Writer: Sent \"%s\"\n", message);

        // Wait for acknowledgment from the reader
        if (read(read_fd, response, BUFSIZE) == -1) {
            perror("writer_thread: Failed to read response");
            close(write_fd);
            close(read_fd);
            pthread_exit(NULL);
        }
        printf("Writer: Received \"%s\"\n", response);
    }

    close(write_fd);
    close(read_fd);
    pthread_exit(NULL);
}

void* reader_thread(void* arg) {
    int read_fd = open(WRITER_TO_READER_FIFO, O_RDONLY);
    int write_fd = open(READER_TO_WRITER_FIFO, O_WRONLY);
    char message[BUFSIZE], response[BUFSIZE];

    if (read_fd == -1 || write_fd == -1) {
        perror("reader_thread: Failed to open FIFOs");
        pthread_exit(NULL);
    }

    for (int i = 0; i < NUM_MESSAGES; i++) {
        // Wait for a message from the writer
        if (read(read_fd, message, BUFSIZE) == -1) {
            perror("reader_thread: Failed to read message");
            close(read_fd);
            close(write_fd);
            pthread_exit(NULL);
        }
        printf("Reader: Received \"%s\"\n", message);

        // Send acknowledgment to the writer
        snprintf(response, BUFSIZE, "Acknowledged: %s", message);
        if (write(write_fd, response, strlen(response) + 1) == -1) {
            perror("reader_thread: Failed to write acknowledgment");
            close(read_fd);
            close(write_fd);
            pthread_exit(NULL);
        }
        printf("Reader: Sent \"%s\"\n", response);
    }

    close(read_fd);
    close(write_fd);
    pthread_exit(NULL);
}
