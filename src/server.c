#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <pthread.h>

#define MAX_CLIENTS 2
#define CATEGORY_SIZE 10

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

void exit_on_wrong_usage(int argc, char *argv[])
{
    // Checks if the argument is supplied upon running the program
    if (argc < 2)
    {
        printf("Usage: %s <PORT_NUMBER>\n", argv[0]);
        exit(1);
    }
}

void send_to(int client_sock, char buffer[])
{
    // Send a message
    int result = send(client_sock, buffer, strlen(buffer), 0);

    if (result < 0)
        exit_with_error("Error: send() Failed.");
}

void pick_category(int c1_sock, int c2_sock)
{
    char *categories[CATEGORY_SIZE] = {"Aquatic Animals", "Filipino Food", "Sports", "Fast Food Restaurants", "Philippine Cities", "Fruits", "ADNU Buildings", "Sneaker Brands", "NBA Teams", "Colors"};
    int random_index = rand() % CATEGORY_SIZE;

    printf("[Server]: The category is: %s\n", categories[random_index]);
    send_to(c1_sock, categories[random_index]);
    send_to(c2_sock, categories[random_index]);
}

void assign_player_roles(int clients[MAX_CLIENTS])
{
    int random_index = rand() % MAX_CLIENTS;
    printf("[Server]: The guesser is: %s\n", (random_index == 0 ? "Client 1" : "Client 2"));

    if (random_index == 0)
    {
        send_to(clients[0], "You are the guesser!");
        send_to(clients[1], "You are the provider!");
    }
    else
    {
        send_to(clients[0], "You are the provider!");
        send_to(clients[1], "You are the guesser!");
    }
}

int main(int argc, char *argv[])
{
    int PORT_NUMBER = atoi(argv[1]);
    int server_sock;
    int clients[MAX_CLIENTS];

    struct sockaddr_in server_addr, client_addr;

    srand(time(NULL));

    exit_on_wrong_usage(argc, argv);

    printf("[Server]: starting at localhost:%s\n", argv[1]);

    // Create a socket for incoming connections
    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock < 0)
        exit_with_error("Error: socket() Failed.");

    // Bind socket to a port
    bzero((char *)&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;          // Internet address
    server_addr.sin_addr.s_addr = INADDR_ANY;  // Any incoming interface
    server_addr.sin_port = htons(PORT_NUMBER); // Local port

    if (bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
        exit_with_error("Error: bind() Failed.");

    // Mark the socket so it will listen for incoming connections
    listen(server_sock, 5);

    // Accept the first connection
    int client_size = sizeof(client_addr);
    clients[0] = accept(server_sock, (struct sockaddr *)&client_addr, &client_size);
    if (clients[0] < 0)
        exit_with_error("Error: accept() Failed.");

    // Accept a second connection
    clients[1] = accept(server_sock, (struct sockaddr *)&client_addr, &client_size);
    if (clients[1] < 0)
        exit_with_error("Error: accept() Failed.");

    bool client_full = (clients[0] != 0 && clients[1] != 0);

    if (client_full)
    {
        printf("[Server]: Game is starting...\n");
        printf("[Server]: Picking category...\n");

        pick_category(clients[0], clients[1]);
        assign_player_roles(clients);
    }

    // Close the sockets
    close_sockets(clients[0], clients[1], server_sock);

    return 0;
}
