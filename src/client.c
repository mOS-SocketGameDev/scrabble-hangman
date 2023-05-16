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

    print(" ");
    print(" %%YClient is %%GStarting...");

    // server connection setup...
    // create a socket for the client
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);

    // set up the server address structure and connect to the server
    setup_server_addr(&server_addr, SERVER_IP_ADDRESS, atoi(SERVER_PORT));
    connect_to_server(client_socket, &server_addr);

    // start of communication/game
    print(" ");
    print(" %%GSuccessfully connected to server at %s:%s.", SERVER_IP_ADDRESS, SERVER_PORT);
    print(" ");

    print("%%C|-------------%%0 %%GSTARTING SCRABBLE OF DEATH%%0 %%C------------|%%0");
    print(" ");

    int current_attempts = MAX_GUESS_ATTEMPTS;

    // get categories
    char category[BUFF_SIZE];

    bzero(category, BUFF_SIZE);
    recv(client_socket, category, BUFF_SIZE, 0);

    // get role
    char role[BUFF_SIZE];

    bzero(role, BUFF_SIZE);
    recv(client_socket, role, BUFF_SIZE, 0);

    for (int i = 0; i < MAX_ROUNDS; i++)
    {
        bool stop = false;

        print("%%C|-----------------------------------------------------|");
        print(" ");
        print(" %%PCATEGORY:%%0 %s", category);
        print(" %%PROLE:%%0 %s", role);
        print(" ");

        if (equal(role, "PROVIDER"))
        {
            char word[BUFF_SIZE];

            bzero(word, BUFF_SIZE);
            printf(" Enter a word: ");
            fgets(word, BUFF_SIZE, stdin);

            send(client_socket, word, BUFF_SIZE, 0);

            char client_res[BUFF_SIZE];
            bzero(client_res, BUFF_SIZE);
            recv(client_socket, client_res, BUFF_SIZE, 0);

            if (equal(client_res, "DONE"))
            {
                strcpy(role, "GUESSER");
                print(" ");
                print(" %%GClient has successfully cleared the round.");
                print(" ");
                continue;
            }
        }

        if (equal(role, "GUESSER"))
        {
            print(" ");
            print(" %%YWaiting for client to provide word...");
            print(" ");
            char word[BUFF_SIZE];

            recv(client_socket, word, BUFF_SIZE, 0);

            char masked_message[BUFF_SIZE];
            hide_word(masked_message, word);

            print(" The word is: %s", masked_message);

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

    close(client_socket);

    return 0;
}