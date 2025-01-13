// File: chat_server.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h> // for memset
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

const char *MY_PORT = "3880"; // Port to listen on
const int BUFLEN = 1000;      // Max buffer size for data

int main()
{
    int rc; // Return code
    int main_socket;
    int serving_socket;
    int fd;
    fd_set rfd;   // Master set of file descriptors
    fd_set c_rfd; // Temporary set for select()
    struct sockaddr_storage her_addr;
    socklen_t her_addr_size;
    struct addrinfo con_kind, *addr_info_res;
    char buf[BUFLEN + 1];
    int max_fd;

    // Initialize the addrinfo struct
    memset(&con_kind, 0, sizeof(con_kind));
    con_kind.ai_family = AF_UNSPEC;     // IPv4 or IPv6
    con_kind.ai_socktype = SOCK_STREAM; // TCP
    con_kind.ai_flags = AI_PASSIVE;     // Automatically fill in my IP

    // Get address info for binding
    if ((rc = getaddrinfo(NULL, MY_PORT, &con_kind, &addr_info_res)) != 0)
    {
        fprintf(stderr, "getaddrinfo() failed: %s\n", gai_strerror(rc));
        exit(EXIT_FAILURE);
    }

    // Create the main socket
    main_socket = socket(addr_info_res->ai_family,
                         addr_info_res->ai_socktype,
                         addr_info_res->ai_protocol);
    if (main_socket < 0)
    {
        perror("socket: allocation failed");
        exit(EXIT_FAILURE);
    }

    // Bind the socket to the address
    rc = bind(main_socket, addr_info_res->ai_addr, addr_info_res->ai_addrlen);
    if (rc)
    {
        perror("bind failed");
        close(main_socket);
        freeaddrinfo(addr_info_res);
        exit(EXIT_FAILURE);
    }

    // Free the address info as it's no longer needed
    freeaddrinfo(addr_info_res);

    // Start listening on the main socket
    rc = listen(main_socket, 5);
    if (rc)
    {
        perror("listen failed");
        close(main_socket);
        exit(EXIT_FAILURE);
    }

    // Initialize the master and temp sets
    FD_ZERO(&rfd);
    FD_SET(main_socket, &rfd); // Add the main socket to the master set
    max_fd = main_socket;      // Keep track of the maximum file descriptor

    printf("Chat server: waiting for connections on port %s...\n", MY_PORT);

    // Main loop to handle incoming connections and data
    while (1)
    {
        c_rfd = rfd; // Copy the master set to the temp set for select()

        // Use select to monitor multiple file descriptors
        rc = select(max_fd + 1, &c_rfd, NULL, NULL, NULL);
        if (rc < 0)
        {
            perror("select failed");
            break;
        }

        // Check if there's a new incoming connection
        if (FD_ISSET(main_socket, &c_rfd))
        {
            her_addr_size = sizeof(her_addr);
            serving_socket = accept(main_socket, (struct sockaddr *)&her_addr, &her_addr_size);
            if (serving_socket >= 0)
            {
                FD_SET(serving_socket, &rfd); // Add the new socket to the master set
                // here we do something different, we keep track of the maximum file descriptor
                // so instead of running till FD_SETSIZE we run till max_fd
                if (serving_socket > max_fd)
                {
                    max_fd = serving_socket; // Update the maximum file descriptor
                }
                printf("Chat server: new connection accepted (fd: %d)\n", serving_socket);
            }
            else
            {
                perror("accept failed");
            }
        }

        // Iterate through all possible file descriptors to check for incoming data
        for (fd = main_socket + 1; fd <= max_fd; fd++)
        {
            if (FD_ISSET(fd, &c_rfd))
            {
                rc = read(fd, buf, BUFLEN);
                if (rc <= 0)
                {
                    if (rc == 0)
                    {
                        // Connection closed by client
                        printf("Chat server: connection closed (fd: %d)\n", fd);
                    }
                    else
                    {
                        perror("read() failed");
                    }
                    close(fd);
                    FD_CLR(fd, &rfd); // Remove from master set
                }
                else
                {
                    buf[rc] = '\0'; // Null-terminate the string
                    printf("Chat server: received '%s' from fd %d\n", buf, fd);

                    // Broadcast the message to all connected clients
                    for (int i = 0; i <= max_fd; i++)
                    {
                        if (FD_ISSET(i, &rfd))
                        {
                            // Don't send the message back to the sender or the main socket
                            if (i != main_socket && i != fd)
                            {
                                // prepare the message
                                char message[BUFLEN * 2];
                                snprintf(message, BUFLEN * 2, "Client %d: %s", fd, buf);
                                size_t message_len = strlen(message) + 1; // +1 for the null terminator
                                if (write(i, message, message_len) < 0)
                                {
                                    perror("write() failed");
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    // Cleanup
    close(main_socket);
    return EXIT_SUCCESS;
}
