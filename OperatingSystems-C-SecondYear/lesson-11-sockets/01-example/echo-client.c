// File: echo_client.c

//run this:
//./echo_client localhost

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // for read/write/close
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

const int BUFLEN = 1024;       // Max buffer size for data
const char SERVER_PORT[] = "3879"; // Port server is listening on

int main(int argc, char *argv[]) {
    int rc; // Return code
    int my_socket;
    int i;
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
    con_kind.ai_family = AF_UNSPEC;     // IPv4 or IPv6
    con_kind.ai_socktype = SOCK_STREAM; // TCP

    // Get address info for the server
    if ((rc = getaddrinfo(argv[1], SERVER_PORT, &con_kind, &addr_info_res)) != 0) {
        fprintf(stderr, "getaddrinfo() failed: %s\n", gai_strerror(rc));
        exit(EXIT_FAILURE);
    }

      // Create the client socket and connect
      //IMPORTANT< cause we using localhost, we need to go over the addr_info_res list, and not just the first one!!!!!!
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
    printf("client connected to server\n");
    printf("addr_info_res->ai_addr: %s\n", addr_info_res->ai_addr);

    /*
    rc = connect(my_socket, addr_info_res->ai_addr, addr_info_res->ai_addrlen);
    if (rc) {
        perror("connect failed");
        close(my_socket);
        freeaddrinfo(addr_info_res);
        exit(EXIT_FAILURE);
    }
    */

    // Free the address info as it's no longer needed
    freeaddrinfo(addr_info_res);

    printf("Echo client: connected to server.\n");

    // Initialize the write buffer
    strcpy(wbuf, "Hello");

    // Send and receive messages in a loop
    for (i = 0; i < 5; i++) { // Send 5 messages
        // Send the message to the server
        rc = write(my_socket, wbuf, strlen(wbuf) + 1); // Include null terminator
        if (rc < 0) {
            perror("write failed");
            close(my_socket);
            exit(EXIT_FAILURE);
        }
        printf("Echo client: sent '%s' to server.\n", wbuf);

        // Receive the echoed message from the server
        rc = read(my_socket, rbuf, BUFLEN);
        if (rc < 0) {
            perror("read failed");
            close(my_socket);
            exit(EXIT_FAILURE);
        } else if (rc == 0) {
            printf("Echo client: server closed the connection.\n");
            break;
        }

        printf("Echo client: received '%s' from server.\n", rbuf);

        // Modify the message for the next iteration
        strcat(wbuf, "!");
        sleep(rand() % 5); // Random sleep between 0-4 seconds
    }

    // Close the socket
    close(my_socket);
    printf("Echo client: connection closed.\n");

    return EXIT_SUCCESS;
}
