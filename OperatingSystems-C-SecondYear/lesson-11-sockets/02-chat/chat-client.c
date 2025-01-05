// File: chat_client.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // for read/write/close
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

#define BUFLEN 1024        // Max buffer size for data
#define SERVER_PORT "3880" // Port server is listening on

int main(int argc, char *argv[]) {
    int rc; // Return code
    int my_socket;
    char rbuf[BUFLEN + 1];
    char wbuf[BUFLEN + 1];
    struct addrinfo con_kind, *addr_info_res;

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

    // Create the client socket
    my_socket = socket(addr_info_res->ai_family,
                       addr_info_res->ai_socktype,
                       addr_info_res->ai_protocol);
    if (my_socket < 0) {
        perror("socket: allocation failed");
        freeaddrinfo(addr_info_res);
        exit(EXIT_FAILURE);
    }

    // Connect to the server
    rc = connect(my_socket, addr_info_res->ai_addr, addr_info_res->ai_addrlen);
    if (rc) {
        perror("connect failed");
        close(my_socket);
        freeaddrinfo(addr_info_res);
        exit(EXIT_FAILURE);
    }

    // Free the address info as it's no longer needed
    freeaddrinfo(addr_info_res);

    printf("Chat client: connected to server.\n");
    printf("Type messages to send. Type 'exit' to quit.\n");

    // Main loop to send and receive messages
    while (1) {
        printf("> ");
        fflush(stdout);

        // Get user input
        if (fgets(wbuf, BUFLEN, stdin) == NULL) {
            perror("fgets failed");
            break;
        }

        // Remove the newline character
        wbuf[strcspn(wbuf, "\n")] = '\0';

        // Check for exit command
        if (strcmp(wbuf, "exit") == 0) {
            printf("Chat client: exiting.\n");
            break;
        }

        // Send the message to the server
        rc = write(my_socket, wbuf, strlen(wbuf) + 1); // Include null terminator
        if (rc < 0) {
            perror("write failed");
            break;
        }

        // Receive the broadcasted message from the server
        rc = read(my_socket, rbuf, BUFLEN);
        if (rc < 0) {
            perror("read failed");
            break;
        } else if (rc == 0) {
            printf("Chat client: server closed the connection.\n");
            break;
        }

        printf("Broadcast: '%s'\n", rbuf);
    }

    // Close the socket
    close(my_socket);
    printf("Chat client: connection closed.\n");

    return EXIT_SUCCESS;
}
