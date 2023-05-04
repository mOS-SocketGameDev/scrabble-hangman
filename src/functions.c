#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <pthread.h>

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

void send_to(int client_sock, char buffer[])
{
    int result = send(client_sock, buffer, strlen(buffer), 0);

    if (result < 0)
        exit_with_error("Error: send() Failed.");
}

void recv_from(int client_sock, char buffer[])
{
    // Set buffer to null
    bzero(buffer, 256);

    // Send a message
    int result = recv(client_sock, buffer, 255, 0);

    if (result < 0)
        exit_with_error("Error: recv() Failed.");
}