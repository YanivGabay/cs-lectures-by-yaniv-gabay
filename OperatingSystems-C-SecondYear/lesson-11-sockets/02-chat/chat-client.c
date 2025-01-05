// File: chat_client.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>     // for read/write/close
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <errno.h>

const int BUFLEN = 1024;          // Max buffer size for data
const char SERVER_PORT[] = "3880"; // Port server is listening on

int main(int argc, char *argv[]) {
    int rc; // Return code
    int my_socket;
    char rbuf[BUFLEN + 1];
    char wbuf[BUFLEN + 1];
    struct addrinfo con_kind, *addr_info_res, *p;
    fd_set master_set, read_fds;
    int fd_max;
    int nbytes;

    // Check for the server hostname argument
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <server_hostname_or_IP>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Initialize the addrinfo struct
    memset(&con_kind, 0, sizeof(con_kind));
    con_kind.ai_family = AF_UNSPEC;      // IPv4 or IPv6
    con_kind.ai_socktype = SOCK_STREAM;  // TCP

    // Get address info for the server
    if ((rc = getaddrinfo(argv[1], SERVER_PORT, &con_kind, &addr_info_res)) != 0) {
        fprintf(stderr, "getaddrinfo() failed: %s\n", gai_strerror(rc));
        exit(EXIT_FAILURE);
    }

    // Create the client socket and connect
    for (p = addr_info_res; p != NULL; p = p->ai_next) {
        my_socket = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (my_socket < 0) {
            perror("socket: allocation failed");
            continue;
        }

        // Optional: Simulate random delay before connecting
        // Commented out to attempt immediate connection
        // sleep(rand() % 10);

        rc = connect(my_socket, p->ai_addr, p->ai_addrlen);
        if (rc == 0) {
            // Successfully connected
            break;
        }

        perror("connect failed");
        close(my_socket);
    }

    if (p == NULL) {
        fprintf(stderr, "Failed to connect to the server.\n");
        freeaddrinfo(addr_info_res);
        exit(EXIT_FAILURE);
    }

    // Free the address info as it's no longer needed
    freeaddrinfo(addr_info_res);

    printf("Chat client: connected to server.\n");
    printf("Type messages to send. Type 'exit' to quit.\n");

    // Initialize the master and temp sets
    FD_ZERO(&master_set);
    FD_SET(STDIN_FILENO, &master_set); // Add stdin to master set
    FD_SET(my_socket, &master_set);     // Add socket to master set
    fd_max = (my_socket > STDIN_FILENO) ? my_socket : STDIN_FILENO;

    // Main loop to send and receive messages
    while (1) {
        read_fds = master_set; // Copy master set to read_fds for select()

        // Use select to monitor multiple file descriptors
        rc = select(fd_max + 1, &read_fds, NULL, NULL, NULL);
        if (rc == -1) {
            perror("select");
            exit(EXIT_FAILURE);
        }

        // Iterate through file descriptors to check which ones are ready
        for (int fd = 0; fd <= fd_max; fd++) {
            if (FD_ISSET(fd, &read_fds)) {
                if (fd == STDIN_FILENO) {
                    // Handle user input
                    if (fgets(wbuf, BUFLEN, stdin) == NULL) {
                        // EOF or error
                        printf("Chat client: exiting.\n");
                        goto cleanup;
                    }

                    // Remove the newline character
                    wbuf[strcspn(wbuf, "\n")] = '\0';

                    // Check for exit command
                    if (strcmp(wbuf, "exit") == 0) {
                        printf("Chat client: exiting.\n");
                        goto cleanup;
                    }

                    // Send the message to the server
                    rc = write(my_socket, wbuf, strlen(wbuf) + 1); // Include null terminator
                    if (rc < 0) {
                        perror("write failed");
                        goto cleanup;
                    }

                } else if (fd == my_socket) {
                    // Handle incoming messages from server
                    nbytes = read(my_socket, rbuf, BUFLEN);
                    if (nbytes <= 0) {
                        if (nbytes == 0) {
                            // Connection closed by server
                            printf("Chat client: server closed the connection.\n");
                        } else {
                            perror("read failed");
                        }
                        goto cleanup;
                    }

                    rbuf[nbytes] = '\0'; // Null-terminate the string
                    printf("\nBroadcast: '%s'\n> ", rbuf);
                    fflush(stdout);
                }
            }
        }
    }

cleanup:
    // Close the socket
    close(my_socket);
    printf("Chat client: connection closed.\n");

    return EXIT_SUCCESS;
}
