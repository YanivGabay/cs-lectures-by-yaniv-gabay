// File: arith_client.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // for read/write/close
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

const int  BUFLEN = 1024;        // Max buffer size for data
const char * SERVER_PORT = "3890"; // Port server is listening on

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

    // Create the client socket and connect
    for (struct addrinfo* p = addr_info_res; p != NULL; p = p->ai_next) {
        my_socket = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (my_socket < 0) {
            perror("socket: allocation failed");
            continue;
        }

      
        rc = connect(my_socket, p->ai_addr, p->ai_addrlen);
        if (rc == 0) {
            // Successfully connected
            break;
        }

        perror("connect failed");
        close(my_socket);
    }

    

    // Free the address info as it's no longer needed
    freeaddrinfo(addr_info_res);

    printf("Arithmetic client: connected to server.\n");
    printf("Enter arithmetic expressions (e.g., '2 + 3'). Type 'exit' to quit.\n");

    // Main loop to send and receive arithmetic expressions
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
            printf("Arithmetic client: exiting.\n");
            break;
        }

        // Send the expression to the server
        rc = write(my_socket, wbuf, strlen(wbuf) + 1); // Include null terminator
        if (rc < 0) {
            perror("write failed");
            break;
        }

        // Receive the result from the server
        rc = read(my_socket, rbuf, BUFLEN);
        if (rc < 0) {
            perror("read failed");
            break;
        } else if (rc == 0) {
            printf("Arithmetic client: server closed the connection.\n");
            break;
        }

        printf("Result: '%s'\n", rbuf);
    }

    // Close the socket
    close(my_socket);
    printf("Arithmetic client: connection closed.\n");

    return EXIT_SUCCESS;
}
