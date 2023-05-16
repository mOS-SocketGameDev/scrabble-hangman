#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <stdarg.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "../include/functions.h"

#define MAX_GUESS_ATTEMPTS 7
#define BUFF_SIZE 255

void print_logo()
{
    print("%%Y|-----------------------------------------------------|%%0");
    print("%%Y|                                                     |%%0");
    print("%%Y|           S C R A B B L E  O F  D E A T H           |%%0");
    print("%%Y|                                                     |%%0");
    print("%%Y|-----------------------------------------------------|%%0");
}

/* HOW TO USE:
 * This works by using:
 *      %%Y -> YELLOW
 *      %%R -> RED
 *      %%G -> GREEN
 *      %%0 -> RESET
 * EXAMPLE:
 *      print("%%Y[Server]: Server is starting...%%0"); -> prints out a yellow line
 * NOTE: make sure to close the color by adding %%0 at the end of which word to stop the coloring
 * EXAMPLE:
 *      print("%%Y[Server]:%%0 Server is starting..."); -> only colors the word "[Server]:"
 */
void print(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    char buffer[1024];
    vsprintf(buffer, format, args);
    va_end(args);

    char color[10] = "\0";
    char *p = buffer;
    while (*p)
    {
        if (*p == '%')
        {
            p++;
            switch (*p)
            {
            case 'R':
                strcpy(color, "\033[31m");
                break;
            case 'G':
                strcpy(color, "\033[32m");
                break;
            case 'Y':
                strcpy(color, "\033[33m");
                break;
            case '0':
                strcpy(color, "\033[0m");
                break;
            default:
                strcpy(color, "");
                break;
            }
            printf("%s", color);
            p++;
        }
        else if (*p == '%' && *(p + 1) == '%')
        {
            p += 2;
            putchar('%');
        }
        else
        {
            putchar(*p);
            p++;
        }
    }

    if (strcmp(color, ""))
        printf("\033[0m");
    printf("\n");
}

void exit_with_error(char *error_msg)
{
    print("%%R%s%%0", error_msg);
    exit(1);
}

void close_sockets(int c1_sock, int s_sock)
{
    close(c1_sock);
    close(s_sock);
}

bool equal(char *s1, char *s2)
{
    return (strcmp(s1, s2) == 0);
}

// connection logic for client
void setup_server_addr(struct sockaddr_in *server_addr, const char *ip_address, int port)
{
    // Clear the server address structure
    memset(server_addr, 0, sizeof(*server_addr));

    // Set up the server address structure
    server_addr->sin_family = AF_INET;
    server_addr->sin_port = htons(port);
    inet_aton(ip_address, &server_addr->sin_addr);
}

void connect_to_server(int client_sock, struct sockaddr_in *server_addr)
{

    if (connect(client_sock, (struct sockaddr *)server_addr, sizeof(*server_addr)) < 0)
    {
        exit_with_error("Error: connect() Failed.");
    }
}

// connection logic for server
void bind_to_server(int server_sock, int port)
{
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    if (bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        exit_with_error("Error: bind() Failed.");
    }

    // Get the dynamically assigned port
    struct sockaddr_in assigned_address;
    socklen_t address_length = sizeof(assigned_address);
    if (getsockname(server_sock, (struct sockaddr *)&assigned_address, &address_length) == -1)
    {
        perror("Failed to get socket name");
        close(server_sock);
    }

    // mark the socket so it will listen for incoming connections
    listen(server_sock, 5);
}

int accept_client(int server_sock, struct sockaddr_in *client_addr)
{
    int client_size = sizeof(*client_addr);
    int client_sock = accept(server_sock, (struct sockaddr *)client_addr, &client_size);
    if (client_sock < 0)
    {
        exit_with_error("Error: accept() Failed.");
    }
    print("%%G[Server]: Client has successfully joined.%%0");
    return client_sock;
}

// global
void draw_hangman(int guess_count)
{
    switch (guess_count)
    {
    case 0:
        print("%%Y_______________________________________________________%%0");
        print("%%Y|                                                     |%%0");
        print("%%Y|%%0      %%G(----)                             (----)%%0      %%Y|%%0");
        print("%%Y|%%0     %%G(%%0%%R*%%0 %%G( )%%0%%R*%%0%%G)%%0     |_______________|     %%G(%%0 %%R*%%0 %%G( )%%0%%R*%%0%%G)%%0    %%Y|%%0");
        print("%%Y|%%0    %%G( (%%0 %%R*%%0%%G) )) )%%0   | %%RH A N G M A N%%0 |    %%G( ( )%%0  %%R*%%0%%G) )%%0   %%Y|%%0");
        print("%%Y|%%0   %%G( ( (%%0 %%R*%%0 %%G)%%0%%R*%%0%%G) )%%0  |---------------|   %%G( ( (%%0 %%R*%%0 %%G)%%0%%R*%%0%%G) )%%0  %%Y|%%0");
        print("%%Y|%%0    %%G( (%%0 %%R*%%0%%G) ) ))%%0   ||             ||    %%G( (%%0 %%R*%%0%%G) ) ))%%0   %%Y|%%0");
        print("%%Y|%%0     %%G( ())%%0 %%R*%%0%%G))%%0    ||             ||     %%G( (())%%0%%R*%%0%%G)%%0     %%Y|%%0");
        print("%%Y|%%0       %%G(%%0||%%G)%%0       ||             ||       %%G(%%0||%%G)%%0       %%Y|%%0");
        print("%%Y|%%0        ||        ||             ||        ||        %%Y|%%0");
        print("%%Y|%%0        ||        ||             ||        ||        %%Y|%%0");
        print("%%Y|%%0--------||-----___||___-------___||___-----||--------%%Y|%%0");
        print("%%Y|_____________________________________________________|%%0");
        print("%%Y|                                                     |%%0");
        print("%%Y|%%0           %%RS C R A B B L E  O F  D E A T H%%0           %%Y|%%0");
        print("%%Y|_____________________________________________________|%%0");
        break;

    case 1:
        print("%%Y_______________________________________________________%%0");
        print("%%Y|                                                     |%%0");
        print("%%Y|%%0      %%G(----)                             (----)%%0      %%Y|%%0");
        print("%%Y|%%0     %%G(%%0%%R*%%0 %%G( )%%0%%R*%%0%%G)%%0     |_______________|     %%G(%%0 %%R*%%0 %%G( )%%0%%R*%%0%%G)%%0    %%Y|%%0");
        print("%%Y|%%0    %%G( (%%0 %%R*%%0%%G) )) )%%0   | %%RH A N G M A N%%0 |    %%G( ( )%%0  %%R*%%0%%G) )%%0   %%Y|%%0");
        print("%%Y|%%0   %%G( ( (%%0 %%R*%%0 %%G)%%0%%R*%%0%%G) )%%0  |---------------|   %%G( ( (%%0 %%R*%%0 %%G)%%0%%R*%%0%%G) )%%0  %%Y|%%0");
        print("%%Y|%%0    %%G( (%%0 %%R*%%0%%G) ) ))%%0   ||             ||    %%G( (%%0 %%R*%%0%%G) ) ))%%0   %%Y|%%0");
        print("%%Y|%%0     %%G( ())%%0 %%R*%%0%%G))%%0    ||             ||     %%G( (())%%0%%R*%%0%%G)%%0     %%Y|%%0");
        print("%%Y|%%0       %%G(%%0||%%G)%%0       ||      %%R|%%0      ||       %%G(%%0||%%G)%%0       %%Y|%%0");
        print("%%Y|%%0        ||        ||             ||        ||        %%Y|%%0");
        print("%%Y|%%0        ||        ||             ||        ||        %%Y|%%0");
        print("%%Y|%%0--------||-----___||___-------___||___-----||--------%%Y|%%0");
        print("%%Y|_____________________________________________________|%%0");
        print("%%Y|                                                     |%%0");
        print("%%Y|%%0           %%RS C R A B B L E  O F  D E A T H%%0           %%Y|%%0");
        print("%%Y|_____________________________________________________|%%0");
        break;
    case 2:
        print("%%Y_______________________________________________________%%0");
        print("%%Y|                                                     |%%0");
        print("%%Y|%%0      %%G(----)                             (----)%%0      %%Y|%%0");
        print("%%Y|%%0     %%G(%%0%%R*%%0 %%G( )%%0%%R*%%0%%G)%%0     |_______________|     %%G(%%0 %%R*%%0 %%G( )%%0%%R*%%0%%G)%%0    %%Y|%%0");
        print("%%Y|%%0    %%G( (%%0 %%R*%%0%%G) )) )%%0   | %%RH A N G M A N%%0 |    %%G( ( )%%0  %%R*%%0%%G) )%%0   %%Y|%%0");
        print("%%Y|%%0   %%G( ( (%%0 %%R*%%0 %%G)%%0%%R*%%0%%G) )%%0  |---------------|   %%G( ( (%%0 %%R*%%0 %%G)%%0%%R*%%0%%G) )%%0  %%Y|%%0");
        print("%%Y|%%0    %%G( (%%0 %%R*%%0%%G) ) ))%%0   ||             ||    %%G( (%%0 %%R*%%0%%G) ) ))%%0   %%Y|%%0");
        print("%%Y|%%0     %%G( ())%%0 %%R*%%0%%G))%%0    ||             ||     %%G( (())%%0%%R*%%0%%G)%%0     %%Y|%%0");
        print("%%Y|%%0       %%G(%%0||%%G)%%0       ||     %%R/|%%0      ||       %%G(%%0||%%G)%%0       %%Y|%%0");
        print("%%Y|%%0        ||        ||             ||        ||        %%Y|%%0");
        print("%%Y|%%0        ||        ||             ||        ||        %%Y|%%0");
        print("%%Y|%%0--------||-----___||___-------___||___-----||--------%%Y|%%0");
        print("%%Y|_____________________________________________________|%%0");
        print("%%Y|                                                     |%%0");
        print("%%Y|%%0           %%RS C R A B B L E  O F  D E A T H%%0           %%Y|%%0");
        print("%%Y|_____________________________________________________|%%0");
        break;
    case 3:
        print("%%Y_______________________________________________________%%0");
        print("%%Y|                                                     |%%0");
        print("%%Y|%%0      %%G(----)                             (----)%%0      %%Y|%%0");
        print("%%Y|%%0     %%G(%%0%%R*%%0 %%G( )%%0%%R*%%0%%G)%%0     |_______________|     %%G(%%0 %%R*%%0 %%G( )%%0%%R*%%0%%G)%%0    %%Y|%%0");
        print("%%Y|%%0    %%G( (%%0 %%R*%%0%%G) )) )%%0   | %%RH A N G M A N%%0 |    %%G( ( )%%0  %%R*%%0%%G) )%%0   %%Y|%%0");
        print("%%Y|%%0   %%G( ( (%%0 %%R*%%0 %%G)%%0%%R*%%0%%G) )%%0  |---------------|   %%G( ( (%%0 %%R*%%0 %%G)%%0%%R*%%0%%G) )%%0  %%Y|%%0");
        print("%%Y|%%0    %%G( (%%0 %%R*%%0%%G) ) ))%%0   ||             ||    %%G( (%%0 %%R*%%0%%G) ) ))%%0   %%Y|%%0");
        print("%%Y|%%0     %%G( ())%%0 %%R*%%0%%G))%%0    ||             ||     %%G( (())%%0%%R*%%0%%G)%%0     %%Y|%%0");
        print("%%Y|%%0       %%G(%%0||%%G)%%0       ||     %%R/|\\%%0     ||       %%G(%%0||%%G)%%0       %%Y|%%0");
        print("%%Y|%%0        ||        ||             ||        ||        %%Y|%%0");
        print("%%Y|%%0        ||        ||             ||        ||        %%Y|%%0");
        print("%%Y|%%0--------||-----___||___-------___||___-----||--------%%Y|%%0");
        print("%%Y|_____________________________________________________|%%0");
        print("%%Y|                                                     |%%0");
        print("%%Y|%%0           %%RS C R A B B L E  O F  D E A T H%%0           %%Y|%%0");
        print("%%Y|_____________________________________________________|%%0");
        break;
    case 4:
        print("%%Y_______________________________________________________%%0");
        print("%%Y|                                                     |%%0");
        print("%%Y|%%0      %%G(----)                             (----)%%0      %%Y|%%0");
        print("%%Y|%%0     %%G(%%0%%R*%%0 %%G( )%%0%%R*%%0%%G)%%0     |_______________|     %%G(%%0 %%R*%%0 %%G( )%%0%%R*%%0%%G)%%0    %%Y|%%0");
        print("%%Y|%%0    %%G( (%%0 %%R*%%0%%G) )) )%%0   | %%RH A N G M A N%%0 |    %%G( ( )%%0  %%R*%%0%%G) )%%0   %%Y|%%0");
        print("%%Y|%%0   %%G( ( (%%0 %%R*%%0 %%G)%%0%%R*%%0%%G) )%%0  |---------------|   %%G( ( (%%0 %%R*%%0 %%G)%%0%%R*%%0%%G) )%%0  %%Y|%%0");
        print("%%Y|%%0    %%G( (%%0 %%R*%%0%%G) ) ))%%0   ||             ||    %%G( (%%0 %%R*%%0%%G) ) ))%%0   %%Y|%%0");
        print("%%Y|%%0     %%G( ())%%0 %%R*%%0%%G))%%0    ||             ||     %%G( (())%%0%%R*%%0%%G)%%0     %%Y|%%0");
        print("%%Y|%%0       %%G(%%0||%%G)%%0       ||     %%R/|\\%%0     ||       %%G(%%0||%%G)%%0       %%Y|%%0");
        print("%%Y|%%0        ||        ||     %%R/%%0       ||        ||        %%Y|%%0");
        print("%%Y|%%0        ||        ||             ||        ||        %%Y|%%0");
        print("%%Y|%%0--------||-----___||___-------___||___-----||--------%%Y|%%0");
        print("%%Y|_____________________________________________________|%%0");
        print("%%Y|                                                     |%%0");
        print("%%Y|%%0           %%RS C R A B B L E  O F  D E A T H%%0           %%Y|%%0");
        print("%%Y|_____________________________________________________|%%0");
        break;
    case 5:
        print("%%Y_______________________________________________________%%0");
        print("%%Y|                                                     |%%0");
        print("%%Y|%%0      %%G(----)                             (----)%%0      %%Y|%%0");
        print("%%Y|%%0     %%G(%%0%%R*%%0 %%G( )%%0%%R*%%0%%G)%%0     |_______________|     %%G(%%0 %%R*%%0 %%G( )%%0%%R*%%0%%G)%%0    %%Y|%%0");
        print("%%Y|%%0    %%G( (%%0 %%R*%%0%%G) )) )%%0   | %%RH A N G M A N%%0 |    %%G( ( )%%0  %%R*%%0%%G) )%%0   %%Y|%%0");
        print("%%Y|%%0   %%G( ( (%%0 %%R*%%0 %%G)%%0%%R*%%0%%G) )%%0  |---------------|   %%G( ( (%%0 %%R*%%0 %%G)%%0%%R*%%0%%G) )%%0  %%Y|%%0");
        print("%%Y|%%0    %%G( (%%0 %%R*%%0%%G) ) ))%%0   ||             ||    %%G( (%%0 %%R*%%0%%G) ) ))%%0   %%Y|%%0");
        print("%%Y|%%0     %%G( ())%%0 %%R*%%0%%G))%%0    ||             ||     %%G( (())%%0%%R*%%0%%G)%%0     %%Y|%%0");
        print("%%Y|%%0       %%G(%%0||%%G)%%0       ||     %%R/|\\%%0     ||       %%G(%%0||%%G)%%0       %%Y|%%0");
        print("%%Y|%%0        ||        ||     %%R/ \\%%0     ||        ||        %%Y|%%0");
        print("%%Y|%%0        ||        ||             ||        ||        %%Y|%%0");
        print("%%Y|%%0--------||-----___||___-------___||___-----||--------%%Y|%%0");
        print("%%Y|_____________________________________________________|%%0");
        print("%%Y|                                                     |%%0");
        print("%%Y|%%0           %%RS C R A B B L E  O F  D E A T H%%0           %%Y|%%0");
        print("%%Y|_____________________________________________________|%%0");
        break;
    case 6:
        print("%%Y_______________________________________________________%%0");
        print("%%Y|                                                     |%%0");
        print("%%Y|%%0      %%G(----)                             (----)%%0      %%Y|%%0");
        print("%%Y|%%0     %%G(%%0%%R*%%0 %%G( )%%0%%R*%%0%%G)%%0     |_______________|     %%G(%%0 %%R*%%0 %%G( )%%0%%R*%%0%%G)%%0    %%Y|%%0");
        print("%%Y|%%0    %%G( (%%0 %%R*%%0%%G) )) )%%0   | %%RH A N G M A N%%0 |    %%G( ( )%%0  %%R*%%0%%G) )%%0   %%Y|%%0");
        print("%%Y|%%0   %%G( ( (%%0 %%R*%%0 %%G)%%0%%R*%%0%%G) )%%0  |---------------|   %%G( ( (%%0 %%R*%%0 %%G)%%0%%R*%%0%%G) )%%0  %%Y|%%0");
        print("%%Y|%%0    %%G( (%%0 %%R*%%0%%G) ) ))%%0   ||             ||    %%G( (%%0 %%R*%%0%%G) ) ))%%0   %%Y|%%0");
        print("%%Y|%%0     %%G( ())%%0 %%R*%%0%%G))%%0    ||       %%Ro%%0     ||     %%G( (())%%0%%R*%%0%%G)%%0     %%Y|%%0");
        print("%%Y|%%0       %%G(%%0||%%G)%%0       ||     %%R/|\\%%0     ||       %%G(%%0||%%G)%%0       %%Y|%%0");
        print("%%Y|%%0        ||        ||     %%R/ \\%%0     ||        ||        %%Y|%%0");
        print("%%Y|%%0        ||        ||             ||        ||        %%Y|%%0");
        print("%%Y|%%0--------||-----___||___-------___||___-----||--------%%Y|%%0");
        print("%%Y|_____________________________________________________|%%0");
        print("%%Y|                                                     |%%0");
        print("%%Y|%%0           %%RS C R A B B L E  O F  D E A T H%%0           %%Y|%%0");
        print("%%Y|_____________________________________________________|%%0");
        break;
    case 7:
        print("%%Y_______________________________________________________%%0");
        print("%%Y|                                                     |%%0");
        print("%%Y|%%0      %%G(----)                             (----)%%0      %%Y|%%0");
        print("%%Y|%%0     %%G(%%0%%R*%%0 %%G( )%%0%%R*%%0%%G)%%0     |_______________|     %%G(%%0 %%R*%%0 %%G( )%%0%%R*%%0%%G)%%0    %%Y|%%0");
        print("%%Y|%%0    %%G( (%%0 %%R*%%0%%G) )) )%%0   | %%RH A N G M A N%%0 |    %%G( ( )%%0  %%R*%%0%%G) )%%0   %%Y|%%0");
        print("%%Y|%%0   %%G( ( (%%0 %%R*%%0 %%G)%%0%%R*%%0%%G) )%%0  |---------------|   %%G( ( (%%0 %%R*%%0 %%G)%%0%%R*%%0%%G) )%%0  %%Y|%%0");
        print("%%Y|%%0    %%G( (%%0 %%R*%%0%%G) ) ))%%0   ||      %%R|%%0      ||    %%G( (%%0 %%R*%%0%%G) ) ))%%0   %%Y|%%0");
        print("%%Y|%%0     %%G( ())%%0 %%R*%%0%%G))%%0    ||       %%Ro%%0     ||     %%G( (())%%0%%R*%%0%%G)%%0     %%Y|%%0");
        print("%%Y|%%0       %%G(%%0||%%G)%%0       ||     %%R/|\\%%0     ||       %%G(%%0||%%G)%%0       %%Y|%%0");
        print("%%Y|%%0        ||        ||     %%R/ \\%%0     ||        ||        %%Y|%%0");
        print("%%Y|%%0        ||        ||             ||        ||        %%Y|%%0");
        print("%%Y|%%0--------||-----___||___-------___||___-----||--------%%Y|%%0");
        print("%%Y|_____________________________________________________|%%0");
        print("%%Y|                                                     |%%0");
        print("%%Y|%%0           %%RS C R A B B L E  O F  D E A T H%%0           %%Y|%%0");
        print("%%Y|_____________________________________________________|%%0");
    default:
        break;
    }
}

bool guess_handler(char word[])
{
    char guessed_word[BUFF_SIZE];
    strcpy(guessed_word, word);

    // Initialize guessedWord with asterisks
    for (int i = 0; i < strlen(guessed_word) - 1; i++)
    {
        guessed_word[i] = '*';
    }

    bool all_characters_guessed = false;
    int wrong_guesses = 0; // Counter for wrong guesses

    while (!all_characters_guessed)
    {
        if (wrong_guesses == MAX_GUESS_ATTEMPTS)
        {
            printf("You lost!\n");
            return false;
        }

        if (strcmp(guessed_word, word) == 0)
        {
            printf("You guessed the word. You won this round!\n");
            return true;
        }

        // guess
        printf("Guess: ");
        char guess;
        scanf(" %c", &guess);

        guess = tolower(guess);

        int found = 0;
        for (int i = 0; i < strlen(guessed_word) - 1; i++)
        {

            if (tolower(word[i]) == guess)
            {
                guessed_word[i] = word[i];
                found = 1;
            }
        }

        printf("The word is: %s\n", guessed_word);

        if (!found)
        {
            wrong_guesses++;
            draw_hangman(wrong_guesses);
            printf("Wrong guess! Remaining attempts: %d\n", (MAX_GUESS_ATTEMPTS - wrong_guesses));
        }
    }
}

void hide_word(char masked_message[], char word[])
{
    strcpy(masked_message, word);
    for (int i = 0; i < strlen(masked_message) - 1; i++)
    {
        if (masked_message[i] != ' ' && masked_message[i] != '\0')
        {
            masked_message[i] = '*';
        }
    }
}
