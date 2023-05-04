#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

void exit_with_error(char *error_msg)
{
    printf("%s\n", error_msg);
    exit(1);
}

void exit_on_wrong_usage(int argc, char *argv[])
{
    // Checks if the argument is supplied upon running the program
    if (argc < 3)
    {
        printf("Usage: %s <SERVER_IP> <PORT_NUMBER>\n", argv[0]);
        exit(1);
    }
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

    char buffer[256], role[256];

    bzero(buffer, 256);

    int result = recv(client_sock, buffer, 255, 0);
    if (result < 0)
        exit_with_error("Error: recv() Failed.");

    printf("The category is: %s\n", buffer);

    int res = recv(client_sock, role, 255, 0);
    if (res < 0)
        exit_with_error("Error: recv() Failed.");

    printf("%s\n", role);

    close(client_sock);

    return 0;
}