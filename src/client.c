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
    // checks if the argument is supplied upon running the program
    if (argc < 3)
    {
        printf("Usage: %s <SERVER_IP> <PORT_NUMBER>\n", argv[0]);
        exit(1);
    }
}

void hide_word(char masked_message[], char word[])
{
    strcpy(masked_message, word);
    for (int i = 0; i < strlen(masked_message) - 1; i++)
    {
        masked_message[i] = '*';
    }
}

int main(int argc, char *argv[])
{
    char *CLIENT_ADDRESS = argv[1],
         *SERVER_PORT = argv[2];
    struct sockaddr_in server_addr;

    exit_on_wrong_usage(argc, argv);

    printf("Client is starting...\n");

    // server connection setup...
    // create a socket for the client
    int client_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (client_sock < 0)
    {
        exit_with_error("Error: socket() Failed.");
    }

    // set up the server address structure
    // and connect to the server
    setup_server_addr(&server_addr, CLIENT_ADDRESS, atoi(SERVER_PORT));
    connect_to_server(client_sock, &server_addr);

    // start of communication/game
    printf("Connected to server at %s:%s.\n", CLIENT_ADDRESS, SERVER_PORT);
    printf("------------------------------------------------\n");
    int current_attempts = 7;
    // init the buffers to be recieved -- category, role.
    char category[BUFF_SIZE], role[BUFF_SIZE];

    // recieve the buffer
    bzero(category, BUFF_SIZE);
    int r_category_res = recv(client_sock, category, BUFF_SIZE, 0);

    // categories is...
    printf("Category: %s\n", category);

    // recieve the buffer
    bzero(role, BUFF_SIZE);
    int role_res = recv(client_sock, role, BUFF_SIZE, 0);

    // your role is...
    printf("Role: %s\n", role);

    char message[BUFF_SIZE];
    if (equal(role, "PROVIDER"))
    {
        // an input from the user
        printf("Enter a word: ");
        bzero(message, BUFF_SIZE);
        fgets(message, BUFF_SIZE, stdin);

        // send to the server
        int s_message_res = send(client_sock, message, BUFF_SIZE, 0);

        // main loop
        while (1)
        {
            // receive the message
            bzero(message, BUFF_SIZE);
            int r_message_res = recv(client_sock, message, BUFF_SIZE, 0);
            printf("Other player guessed: %s", message);
        }
    }
    if (equal(role, "GUESSER"))
    {
        // receive the message
        bzero(message, BUFF_SIZE);
        int r_message_res = recv(client_sock, message, BUFF_SIZE, 0);

        // mask the word
        char masked_message[BUFF_SIZE];
        hide_word(masked_message, message);
        printf("The word is: %s\n", masked_message);

        // main loop
        while (1)
        {
            // an input from the user
            printf("Guess: ");
            bzero(message, BUFF_SIZE);
            fgets(message, BUFF_SIZE, stdin);

            // send to the server
            int s_message_res = send(client_sock, message, BUFF_SIZE, 0);
        }
    }

    close(client_sock);

    return 0;
}
