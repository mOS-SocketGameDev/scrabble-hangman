#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "../include/functions.h"

#define BUFF_SIZE 255
#define MAX_ROUNDS 4

void exit_on_wrong_usage(int argc, char *argv[])
{
    // Checks if the argument is supplied upon running the program
    if (argc < 3)
    {
        printf("Usage: %s <SERVER_IP> <PORT_NUMBER>\n", argv[0]);
        exit(1);
    }
}

int main(int argc, char *argv[])
{
    struct sockaddr_in server_addr;

    exit_on_wrong_usage(argc, argv);

    printf("Client is starting...\n");

    // Create a socket for the client
    int client_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (client_sock < 0)
        exit_with_error("Error: socket() Failed.");

    // Set up the server address structure
    bzero((char *)&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[2]));
    inet_aton(argv[1], &server_addr.sin_addr);

    // Connect to the server
    if (connect(client_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
        exit_with_error("Error: connect() Failed.");

    // Start of communication
    printf("Connected to server at %s:%s.\n", argv[1], argv[2]);

    // init the buffers to be recieved -- category, role.
    char category[BUFF_SIZE], role[BUFF_SIZE];

    // recieve the buffer
    bzero(category, BUFF_SIZE);
    int category_res = recv(client_sock, category, BUFF_SIZE, 0);

    // categories is...
    printf("Category: %s\n", category);

    // recieve the buffer
    bzero(role, BUFF_SIZE);
    int buffer2_res = recv(client_sock, role, BUFF_SIZE, 0);

    // your role is...
    printf("Role: %s\n", role);

    if (strcmp(role, "GUESSER") == 0)
    {
    }
    if (strcmp(role, "PROVIDER") == 0)
    {
    }

    close(client_sock);

    return 0;
}
