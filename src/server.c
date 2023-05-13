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

#define MAX_CLIENTS 2
#define CATEGORY_SIZE 10
#define BUFF_SIZE 255

void exit_on_wrong_usage(int argc, char *argv[])
{
    // Checks if the argument is supplied upon running the program
    if (argc < 2)
    {
        printf("Usage: %s <PORT_NUMBER>\n", argv[0]);
        exit(1);
    }
}

void pick_category(int c1_sock, int c2_sock)
{
    char *categories[CATEGORY_SIZE] =
        {
            "Aquatic Animals",
            "Filipino Food",
            "Sports",
            "Fast Food Restaurants",
            "Philippine Cities",
            "Fruits",
            "ADNU Buildings",
            "Sneaker Brands",
            "NBA Teams",
            "Colors",
        };

    int random_index = rand() % CATEGORY_SIZE;

    printf("[Server]: The category is: %s\n", categories[random_index]);

    // send 2 buffers
    int res1_1 = send(c1_sock, categories[random_index], BUFF_SIZE, 0);
    int res1_2 = send(c2_sock, categories[random_index], BUFF_SIZE, 0);
}

int assign_player_roles(int c1_sock, int c2_sock)
{
    int random_index = rand() % MAX_CLIENTS;
    int s_category0_rs, s_category1_rs;

    if (random_index == 0)
    {
        printf("[Server]: Client 1 is the Guesser!\n");
        printf("[Server]: Client 2 is the Provider!\n");
        // send 2 buffers
        s_category0_rs = send(c1_sock, "GUESSER", BUFF_SIZE, 0);
        s_category1_rs = send(c2_sock, "PROVIDER", BUFF_SIZE, 0);
    }
    else
    {
        printf("[Server]: Client 1 is the Provider!\n");
        printf("[Server]: Client 2 is the Guesser!\n");
        // send 2 buffers
        s_category0_rs = send(c1_sock, "PROVIDER", BUFF_SIZE, 0);
        s_category1_rs = send(c2_sock, "GUESSER", BUFF_SIZE, 0);
    }

    if (s_category0_rs < 0 || s_category1_rs < 0)
    {
        exit_with_error("Sending categories failed.");
    }

    // return which one is the guesser
    return random_index;
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

    // create a socket for incoming connections
    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock < 0)
        exit_with_error("Error: socket() Failed.");

    // bind socket to a port
    bzero((char *)&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;          // Internet address
    server_addr.sin_addr.s_addr = INADDR_ANY;  // Any incoming interface
    server_addr.sin_port = htons(PORT_NUMBER); // Local port

    if (bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
        exit_with_error("Error: bind() Failed.");

    // mark the socket so it will listen for incoming connections
    listen(server_sock, 5);

    // accept the first connection
    int client_size = sizeof(client_addr);
    clients[0] = accept(server_sock, (struct sockaddr *)&client_addr, &client_size);
    if (clients[0] < 0)
        exit_with_error("Error: accept() Failed.");

    printf("[Server]: Client 1 has successfully joined.\n");

    // accept a second connection
    clients[1] = accept(server_sock, (struct sockaddr *)&client_addr, &client_size);
    if (clients[1] < 0)
        exit_with_error("Error: accept() Failed.");

    printf("[Server]: Client 2 has successfully joined.\n");

    bool is_client_full = (clients[0] != 0 && clients[1] != 0);

    if (is_client_full)
    {
        // initialize game configs
        printf("[Server]: Game is starting...\n");

        pick_category(clients[0], clients[1]);
        int guesser_res = assign_player_roles(clients[0], clients[1]);

        // handle the communication here...
        char message[BUFF_SIZE];
        int recv_to = (guesser_res == 0) ? 1 : 0;
        int send_to = (recv_to == 0) ? 1 : 0;

        // recieve the first word and send to both clients
        bzero(message, BUFF_SIZE);
        int r_message0_res = recv(clients[recv_to], message, BUFF_SIZE, 0);
        int s_message0_res = send(clients[send_to], message, BUFF_SIZE, 0);

        while (1)
        {
            // send/recieve the clients guesses
            bzero(message, BUFF_SIZE);
            int r_message1_res = recv(clients[send_to], message, BUFF_SIZE, 0);
            int s_message1_res = send(clients[recv_to], message, BUFF_SIZE, 0);
        }
    }

    // close the sockets
    close_sockets(clients[0], clients[1], server_sock);

    return 0;
}
