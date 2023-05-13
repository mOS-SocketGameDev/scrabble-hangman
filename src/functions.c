#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>

#include "../include/functions.h"

void exit_with_error(char *error_msg)
{
    printf("%s\n", error_msg);
    exit(1);
}

void close_sockets(int c1_sock, int c2_sock, int s_sock)
{
    close(c1_sock);
    close(c2_sock);
    close(s_sock);
}

bool equal(char *s1, char *s2)
{
    return (strcmp(s1, s2) == 0);
}

// connection logic for client
void setup_server_addr(struct sockaddr_in *server_addr, const char *ip_address, int port)
{
    // Clear the server address structure
    memset(server_addr, 0, sizeof(*server_addr));

    // Set up the server address structure
    server_addr->sin_family = AF_INET;
    server_addr->sin_port = htons(port);
    inet_aton(ip_address, &server_addr->sin_addr);
}

void connect_to_server(int client_sock, struct sockaddr_in *server_addr)
{

    if (connect(client_sock, (struct sockaddr *)server_addr, sizeof(*server_addr)) < 0)
    {
        perror("Error: connect() Failed.");
    }
}

// connection logic for server
void bind_to_server(int server_sock, int port)
{
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    if (bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("Error: bind() Failed.");
        exit(EXIT_FAILURE);
    }

    // mark the socket so it will listen for incoming connections
    listen(server_sock, 5);
}

int accept_client(int server_sock, struct sockaddr_in *client_addr)
{
    int client_size = sizeof(*client_addr);
    int client_sock = accept(server_sock, (struct sockaddr *)client_addr, &client_size);
    if (client_sock < 0)
    {
        perror("Error: accept() Failed.");
        exit(EXIT_FAILURE);
    }
    printf("[Server]: Client has successfully joined.\n");
    return client_sock;
}