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
    for (int i = 0; i < strlen(masked_message); i++)
    {
        masked_message[i] = '*';
    }
}

int main(int argc, char *argv[])
{
    struct sockaddr_in server_addr;

    exit_on_wrong_usage(argc, argv);

    printf("Client is starting...\n");

    // create a socket for the client
    int client_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (client_sock < 0)
        exit_with_error("Error: socket() Failed.");

    // set up the server address structure
    bzero((char *)&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[2]));
    inet_aton(argv[1], &server_addr.sin_addr);

    // connect to the server
    if (connect(client_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
        exit_with_error("Error: connect() Failed.");

    // start of communication/game
    printf("Connected to server at %s:%s.\n", argv[1], argv[2]);
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
