// File: echo_server.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h> // for memset
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <signal.h>
const int BUFLEN = 1024;       // Max buffer size for data
const char SERVER_PORT[] = "3879"; // Port server is listening on

int main_socket; // THE primary listening socket

void handler(int sig){
    printf("Signal %d received\n", sig);
    close(main_socket);
    exit(EXIT_SUCCESS);
}

int main() {
    int rc; // Return code
    signal(SIGINT, handler);
    int serving_socket; // represent a new connection
    int fd;
    fd_set rfd;    // a set containing all active file descriptors (connections)
    fd_set c_rfd;  // Temporary set for select()
    struct sockaddr_storage her_addr; // holds client address
    socklen_t her_addr_size;
    struct addrinfo con_kind, *addr_info_res; // setting up server address
    char buf[BUFLEN + 1];

    // Initialize the addrinfo struct
    memset(&con_kind, 0, sizeof(con_kind));
    con_kind.ai_family = AF_UNSPEC;      // IPv4 or IPv6
    con_kind.ai_socktype = SOCK_STREAM;  // TCP
    con_kind.ai_flags = AI_PASSIVE;      // Automatically fill in my IP

    // Get address info for binding
    if ((rc = getaddrinfo(NULL, SERVER_PORT, &con_kind, &addr_info_res)) != 0) {
        fprintf(stderr, "getaddrinfo() failed: %s\n", gai_strerror(rc));
        exit(EXIT_FAILURE);
    }

    // Create the main socket
    main_socket = socket(addr_info_res->ai_family,
                         addr_info_res->ai_socktype,
                         addr_info_res->ai_protocol);
    if (main_socket < 0) {
        perror("socket: allocation failed");
        exit(EXIT_FAILURE);
    }

    // Bind the socket to the address
    rc = bind(main_socket, addr_info_res->ai_addr, addr_info_res->ai_addrlen);
    //basicly tells the socket, your ip port and address are what we got from the getaddrinfo
    if (rc) {
        perror("bind failed");
        close(main_socket);
        freeaddrinfo(addr_info_res);
        exit(EXIT_FAILURE);
    }

    // Free the address info as it's no longer needed
    freeaddrinfo(addr_info_res);

    // Start listening on the main socket
    rc = listen(main_socket, 5);
    if (rc) {
        perror("listen failed");
        close(main_socket);
        exit(EXIT_FAILURE);
    }

    /*
   




   This comment indicates that the following lines of code are initializing the 
   file descriptor sets.

 **FD_ZERO(&rfd)**:
  
   This macro initializes the file descriptor set `rfd` to be an empty set. 
   `FD_ZERO` is used to clear a set of file descriptors, 
   ensuring that it contains no file descriptors before we start adding to it.

 **FD_SET(main_socket, &rfd)**:
  
   This macro adds the file descriptor `main_socket` to the set `rfd`.
    `FD_SET` is used to include a specific file descriptor in the set.
     Here, `main_socket` is the socket file descriptor that the server is
      using to listen for incoming connections.

### Summary
- `FD_ZERO(&rfd)` clears the set `rfd`.
- `FD_SET(main_socket, &rfd)` adds the `main_socket` file descriptor to the set `rfd`.

These steps are typically part of setting up the `select` system call,
 which monitors multiple file descriptors to see if any of them are ready 
 for I/O operations (e.g., reading or writing).
 By initializing and setting up the file descriptor set, 
 the server can efficiently manage multiple connections.
    */

    // Initialize the master and temp sets
    FD_ZERO(&rfd); // think of rfd as a group of telephone lines that the server is listening on
    // at the start, only the main_socket is in the group
    FD_SET(main_socket, &rfd); // Add the main socket to the master set

    printf("Echo server: waiting for connections on port %s...\n", SERVER_PORT);

    // Main loop to handle incoming connections and data
    while (1) {
        c_rfd = rfd; // Copy the master set to the temp set for select()

        // Use select to monitor multiple file descriptors
        //think of select like a traffic controller, it will tell the server which of the telephone lines in the group rfd has a call

        //I HIGHLY recommend you not to use gettablesize() as for me it got errors, instead use FD_SETSIZE
        //or as you can see in the next examples you can keep track of the max fd
        rc = select(FD_SETSIZE, &c_rfd, NULL, NULL, NULL);
        if (rc < 0) {
            perror("select failed");
            break;
        }

        // Check if there's a new incoming connection
        if (FD_ISSET(main_socket, &c_rfd)) {
            her_addr_size = sizeof(her_addr);
            serving_socket = accept(main_socket, (struct sockaddr *)&her_addr, &her_addr_size);
            if (serving_socket >= 0) {
                FD_SET(serving_socket, &rfd); // Add the new socket to the master set
                printf("Echo server: new connection accepted (fd: %d)\n", serving_socket);
            } else {
                perror("accept failed");
            }
        }

        // Iterate through all possible file descriptors to check for incoming data
        // again here, use the same FD_SETSIZE and not gettablesize()
        for (fd = main_socket + 1; fd < FD_SETSIZE; fd++) {
            if (FD_ISSET(fd, &c_rfd)) {
                printf("Echo server: data available on fd %d\n", fd);
                rc = read(fd, buf, BUFLEN);
                if (rc == 0) {
                    // Connection closed by client
                    printf("Echo server: connection closed (fd: %d)\n", fd);
                    close(fd);
                    FD_CLR(fd, &rfd); // Remove from master set
                } else if (rc > 0) {
                    // Echo the received data back to the client
                    buf[rc] = '\0'; // Null-terminate the string
                    printf("Echo server: received '%s' from fd %d\n", buf, fd);
                    write(fd, buf, rc);
                } else {
                    perror("read() failed");
                    close(fd);
                    FD_CLR(fd, &rfd);
                }
            }
        }
    }

    // Cleanup
    close(main_socket);
    return EXIT_SUCCESS;
}
