#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <pthread.h>
#include <ctype.h>

#include "../include/functions.h"

#define MAX_ROUNDS 4
#define MAX_GUESS_ATTEMPTS 7
#define MAX_CLIENTS 4
#define CATEGORY_SIZE 10
#define BUFF_SIZE 255

void exit_on_wrong_usage(int argc, char *argv[])
{
    // Checks if the argument is supplied upon running the program
    if (argc < 2)
    {
        print("Usage: %s <PORT_NUMBER>", argv[0]);
        exit(1);
    }
}

char *pick_category(int c1_sock)
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

    send(c1_sock, categories[random_index], BUFF_SIZE, 0);
    return categories[random_index];
}

int pick_role(int c1_sock)
{
    int random_index = rand() % MAX_CLIENTS;

    // 0 => server
    // 1 => client
    char *role = (random_index == 0) ? "GUESSER" : "PROVIDER";
    send(c1_sock, role, BUFF_SIZE, 0);

    return random_index;
}

int main(int argc, char *argv[])
{
    srand(time(NULL));
    print_logo();

    int PORT_NUMBER = atoi(argv[1]);

    struct sockaddr_in server_addr, client_addr;

    exit_on_wrong_usage(argc, argv);

    // create a socket for incoming connections
    int server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock < 0)
    {
        exit_with_error("Error: socket() Failed.");
    }

    // bind socket to a port
    bind_to_server(server_sock, PORT_NUMBER);
    print("Server is starting...");
    print("Waiting for connections...");

    // accept the first connection
    memset(&client_addr, 0, sizeof(client_addr));
    int client_socket = accept_client(server_sock, &client_addr);
    if (client_socket < 0)
    {
        exit_with_error("Error: accept() Failed.");
    }

    print("Client successfully connected.");

    // start game
    print("--- Starting Game ---");

    int current_attempts = MAX_GUESS_ATTEMPTS;

    // send category
    char *category = pick_category(client_socket);
    int roleNo = pick_role(client_socket);

    char role[BUFF_SIZE];
    strcpy(role, (roleNo == 0) ? "PROVIDER" : "GUESSER");

    for (int i = 0; i < MAX_ROUNDS; i++)
    {
        print("--------------------------------------");
        print("CATEGORY: %s", category);
        print("ROLE: %s", role);

        if (equal(role, "PROVIDER"))
        {
            char word[BUFF_SIZE];

            bzero(word, BUFF_SIZE);
            printf("Enter a word: ");
            fgets(word, BUFF_SIZE, stdin);

            send(client_socket, word, BUFF_SIZE, 0);

            char client_res[BUFF_SIZE];
            bzero(client_res, BUFF_SIZE);
            recv(client_socket, client_res, BUFF_SIZE, 0);

            if (equal(client_res, "DONE"))
            {
                strcpy(role, "GUESSER");
                print("Client has successfully cleared the round.");
                continue;
            }
        }

        if (equal(role, "GUESSER"))
        {
            print("Waiting for client to provide word...");
            char word[BUFF_SIZE];

            recv(client_socket, word, BUFF_SIZE, 0);

            char masked_message[BUFF_SIZE];
            hide_word(masked_message, word);

            print("The word is: %s", masked_message);

            char guess[BUFF_SIZE];
            while (1)
            {
                bool res = guess_handler(word);

                if (res)
                {
                    // signal the server that the round is finished
                    // time to swap
                    strcpy(role, "PROVIDER");

                    send(client_socket, "DONE", BUFF_SIZE, 0);
                    break;
                }
            }
        }
    }

    // close the sockets
    close_sockets(client_socket, server_sock);

    return 0;
}
