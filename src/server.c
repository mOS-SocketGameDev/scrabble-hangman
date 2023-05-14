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

#define MAX_GUESS_ATTEMPTS 7
#define MAX_CLIENTS 2
#define CATEGORY_SIZE 10
#define BUFF_SIZE 255

void exit_on_wrong_usage(int argc, char *argv[])
{
    // Checks if the argument is supplied upon running the program
    if (argc < 2)
    {
        print("%%RUsage: %s <PORT_NUMBER>%%0", argv[0]);
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

    print("%%G[Server]: The category is: %s%%0", categories[random_index]);

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
        print("%%Y[Server]: Client 1 is the Guesser!%%0");
        print("%%Y[Server]: Client 2 is the Provider!%%0");
        // send 2 buffers
        s_category0_rs = send(c1_sock, "GUESSER", BUFF_SIZE, 0);
        s_category1_rs = send(c2_sock, "PROVIDER", BUFF_SIZE, 0);
    }
    else
    {
        print("%%Y[Server]: Client 1 is the Provider!%%0");
        print("%%Y[Server]: Client 2 is the Guesser!%%0");
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
    print_logo();

    int PORT_NUMBER = atoi(argv[1]);
    int server_sock;
    int clients[MAX_CLIENTS];

    struct sockaddr_in server_addr, client_addr;

    srand(time(NULL));

    exit_on_wrong_usage(argc, argv);

    print("%%Y[Server]: starting at localhost:%s%%0", argv[1]);

    // create a socket for incoming connections
    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock < 0)
    {
        exit_with_error("Error: socket() Failed.");
    }

    // bind socket to a port
    bind_to_server(server_sock, PORT_NUMBER);

    // accept the first connection
    memset(&client_addr, 0, sizeof(client_addr));
    clients[0] = accept_client(server_sock, &client_addr);
    clients[1] = accept_client(server_sock, &client_addr);

    bool is_client_full = (clients[0] != 0 && clients[1] != 0);

    if (is_client_full)
    {
        // initialize game configs
        print("%%G[Server]: Game is starting...%%0");

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

        int attempts = 0;
        while (attempts != MAX_GUESS_ATTEMPTS)
        {
            // send/recieve the clients guesses
            bzero(message, BUFF_SIZE);
            int r_message1_res = recv(clients[send_to], message, BUFF_SIZE, 0);
            int s_message1_res = send(clients[recv_to], message, BUFF_SIZE, 0);

            attempts++;

            if (attempts == MAX_GUESS_ATTEMPTS)
            {
                print("%%R[Server]: Guesser lose.%%0");
                close_sockets(clients[0], clients[1], server_sock);
            }
        }
    }

    // close the sockets
    close_sockets(clients[0], clients[1], server_sock);

    return 0;
}
