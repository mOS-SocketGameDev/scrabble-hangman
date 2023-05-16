#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <ctype.h>

#include "../include/functions.h"

#define BUFF_SIZE 255
#define MAX_ROUNDS 4
#define MAX_GUESS_ATTEMPTS 7
#define MAX_WORD_LENGTH 10
#define MAX_GUESSES 7

void exit_on_wrong_usage(int argc, char *argv[])
{
    // checks if the argument is supplied upon running the program
    if (argc < 3)
    {
        print("%%RUsage: %s <SERVER_IP> <PORT_NUMBER>", argv[0]);
        exit(1);
    }
}

int main(int argc, char *argv[])
{
    print_logo();

    char *SERVER_IP_ADDRESS = argv[1],
         *SERVER_PORT = argv[2];
    struct sockaddr_in server_addr;

    exit_on_wrong_usage(argc, argv);

    print("%%YClient is starting...");

    // server connection setup...
    // create a socket for the client
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);

    // set up the server address structure and connect to the server
    setup_server_addr(&server_addr, SERVER_IP_ADDRESS, atoi(SERVER_PORT));
    connect_to_server(client_socket, &server_addr);

    // start of communication/game
    print("Successfully connected to server at %s:%s.", SERVER_IP_ADDRESS, SERVER_PORT);

    print("--- Starting Game ---");

    int current_attempts = MAX_GUESS_ATTEMPTS;

    char category[BUFF_SIZE];
    char role[BUFF_SIZE];

    for (int i = 0; i < MAX_ROUNDS; i++)
    {
        // get categories
        bzero(category, BUFF_SIZE);
        recv(client_socket, category, BUFF_SIZE, 0);
        print("CATEGORY: %s", category);

        // get role
        bzero(role, BUFF_SIZE);
        recv(client_socket, role, BUFF_SIZE, 0);
        print("ROLE: %s", role);

        if (equal(role, "GUESSER"))
        {
            char word[BUFF_SIZE];

            recv(client_socket, word, BUFF_SIZE, 0);

            char masked_message[BUFF_SIZE];
            hide_word(masked_message, word);

            print("The word is: %s", masked_message);

            char guess[BUFF_SIZE];
            while (current_attempts > 0)
            {
                bzero(guess, BUFF_SIZE);
                printf("Guess: ");
                fgets(guess, BUFF_SIZE, stdin);

                bool res = guess_handler(word);

                if (res)
                {
                    // swap
                    strcpy(role, "PROVIDER");
                    break;
                }
            }
        }
        if (equal(role, "PROVIDER"))
        {
            char buffer[BUFF_SIZE];

            bzero(buffer, BUFF_SIZE);
            printf("Enter a word: ");
            fgets(buffer, BUFF_SIZE, stdin);

            send(client_socket, buffer, BUFF_SIZE, 0);
        }
    }

    close(client_socket);

    return 0;
}
