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
        print("%%RUsage: %s <SERVER_IP> <PORT_NUMBER>%%0", argv[0]);
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

void draw_hangman(int guess_count)
{
    switch (guess_count)
    {
    case 0:
        print("_______________________________________________________");
        print("|                                                     |");
        print("|      (----)	 	                    (----)      |");
        print("|     (* ( )*)     |_______________|     (* ( )*)     |");
        print("|    ( (*) )) )    | H A N G M A N |    ( ( ) *) )    |");
        print("|   ( ( (* )*) )   |---------------|   ( ( (* )*) )   |");
        print("|    ( (*) ) ))    ||             ||    ( (*) ) ))    |");
        print("|     ( ())*))     ||             ||     ( (())*)     |");
        print("|       (||)       ||             ||       (||)       |");
        print("|        ||        ||             ||        ||        |");
        print("|        ||        ||             ||        ||        |");
        print("|--------||-----___||___-------___||___-----||--------|");
        print("|_____________________________________________________|");
        print("|                                                     |");
        print("|           S C R A B B L E  O F  D E A T H           |");
        print("|_____________________________________________________|");
        break;
    case 1:
        print("_______________________________________________________");
        print("|                                                     |");
        print("|      (----)	 	                    (----)      |");
        print("|     (* ( )*)     |_______________|     (* ( )*)     |");
        print("|    ( (*) )) )    | H A N G M A N |    ( ( ) *) )    |");
        print("|   ( ( (* )*) )   |---------------|   ( ( (* )*) )   |");
        print("|    ( (*) ) ))    ||             ||    ( (*) ) ))    |");
        print("|     ( ())*))     ||             ||     ( (())*)     |");
        print("|       (||)       ||      |      ||       (||)       |");
        print("|        ||        ||             ||        ||        |");
        print("|        ||        ||             ||        ||        |");
        print("|--------||-----___||___-------___||___-----||--------|");
        print("|_____________________________________________________|");
        print("|                                                     |");
        print("|           S C R A B B L E  O F  D E A T H           |");
        print("|_____________________________________________________|");
        break;
    case 2:
        print("_______________________________________________________");
        print("|                                                     |");
        print("|      (----)	 	                    (----)      |");
        print("|     (* ( )*)     |_______________|     (* ( )*)     |");
        print("|    ( (*) )) )    | H A N G M A N |    ( ( ) *) )    |");
        print("|   ( ( (* )*) )   |---------------|   ( ( (* )*) )   |");
        print("|    ( (*) ) ))    ||             ||    ( (*) ) ))    |");
        print("|     ( ())*))     ||             ||     ( (())*)     |");
        print("|       (||)       ||     /|      ||       (||)       |");
        print("|        ||        ||             ||        ||        |");
        print("|        ||        ||             ||        ||        |");
        print("|--------||-----___||___-------___||___-----||--------|");
        print("|_____________________________________________________|");
        print("|                                                     |");
        print("|           S C R A B B L E  O F  D E A T H           |");
        print("|_____________________________________________________|");
        break;
    case 3:
        print("_______________________________________________________");
        print("|                                                     |");
        print("|      (----)	 	                    (----)      |");
        print("|     (* ( )*)     |_______________|     (* ( )*)     |");
        print("|    ( (*) )) )    | H A N G M A N |    ( ( ) *) )    |");
        print("|   ( ( (* )*) )   |---------------|   ( ( (* )*) )   |");
        print("|    ( (*) ) ))    ||             ||    ( (*) ) ))    |");
        print("|     ( ())*))     ||             ||     ( (())*)     |");
        print("|       (||)       ||     /|\\     ||       (||)       |");
        print("|        ||        ||             ||        ||        |");
        print("|        ||        ||             ||        ||        |");
        print("|--------||-----___||___-------___||___-----||--------|");
        print("|_____________________________________________________|");
        print("|                                                     |");
        print("|           S C R A B B L E  O F  D E A T H           |");
        print("|_____________________________________________________|");
        break;
    case 4:
        print("_______________________________________________________");
        print("|                                                     |");
        print("|      (----)	 	                    (----)      |");
        print("|     (* ( )*)     |_______________|     (* ( )*)     |");
        print("|    ( (*) )) )    | H A N G M A N |    ( ( ) *) )    |");
        print("|   ( ( (* )*) )   |---------------|   ( ( (* )*) )   |");
        print("|    ( (*) ) ))    ||             ||    ( (*) ) ))    |");
        print("|     ( ())*))     ||             ||     ( (())*)     |");
        print("|       (||)       ||     /|\\     ||       (||)       |");
        print("|        ||        ||     /       ||        ||        |");
        print("|        ||        ||             ||        ||        |");
        print("|--------||-----___||___-------___||___-----||--------|");
        print("|_____________________________________________________|");
        print("|                                                     |");
        print("|           S C R A B B L E  O F  D E A T H           |");
        print("|_____________________________________________________|");
        break;
    case 5:
        print("_______________________________________________________");
        print("|                                                     |");
        print("|      (----)	 	                    (----)      |");
        print("|     (* ( )*)     |_______________|     (* ( )*)     |");
        print("|    ( (*) )) )    | H A N G M A N |    ( ( ) *) )    |");
        print("|   ( ( (* )*) )   |---------------|   ( ( (* )*) )   |");
        print("|    ( (*) ) ))    ||             ||    ( (*) ) ))    |");
        print("|     ( ())*))     ||             ||     ( (())*)     |");
        print("|       (||)       ||     /|\\     ||       (||)       |");
        print("|        ||        ||     / \\     ||        ||        |");
        print("|        ||        ||             ||        ||        |");
        print("|--------||-----___||___-------___||___-----||--------|");
        print("|_____________________________________________________|");
        print("|                                                     |");
        print("|           S C R A B B L E  O F  D E A T H           |");
        print("|_____________________________________________________|");
        break;
    case 6:
        print("_______________________________________________________");
        print("|                                                     |");
        print("|      (----)	 	                    (----)      |");
        print("|     (* ( )*)     |_______________|     (* ( )*)     |");
        print("|    ( (*) )) )    | H A N G M A N |    ( ( ) *) )    |");
        print("|   ( ( (* )*) )   |---------------|   ( ( (* )*) )   |");
        print("|    ( (*) ) ))    ||             ||    ( (*) ) ))    |");
        print("|     ( ())*))     ||       o     ||     ( (())*)     |");
        print("|       (||)       ||     /|\\     ||       (||)       |");
        print("|        ||        ||     / \\     ||        ||        |");
        print("|        ||        ||             ||        ||        |");
        print("|--------||-----___||___-------___||___-----||--------|");
        print("|_____________________________________________________|");
        print("|                                                     |");
        print("|           S C R A B B L E  O F  D E A T H           |");
        print("|_____________________________________________________|");
        break;
    case 7:
        print("_______________________________________________________");
        print("|                                                     |");
        print("|      (----)	 	                    (----)      |");
        print("|     (* ( )*)     |_______________|     (* ( )*)     |");
        print("|    ( (*) )) )    | H A N G M A N |    ( ( ) *) )    |");
        print("|   ( ( (* )*) )   |---------------|   ( ( (* )*) )   |");
        print("|    ( (*) ) ))    ||      |      ||    ( (*) ) ))    |");
        print("|     ( ())*))     ||       o     ||     ( (())*)     |");
        print("|       (||)       ||     /|\\     ||       (||)       |");
        print("|        ||        ||     / \\     ||        ||        |");
        print("|        ||        ||             ||        ||        |");
        print("|--------||-----___||___-------___||___-----||--------|");
        print("|_____________________________________________________|");
        print("|                                                     |");
        print("|           S C R A B B L E  O F  D E A T H           |");
        print("|_____________________________________________________|");
    default:
        break;
    }
}

// Take a guess from the user
char guess(char word[])
{
    char guessed_word[strlen(word)];

    // Initialize guessedWord with asterisks
    for (int i = 0; i < strlen(word); i++)
    {
        guessed_word[i] = '*';
    }
    guessed_word[strlen(word)] = '\0';

    bool all_characters_guessed = false;
    int wrong_guesses = 0; // Counter for wrong guesses

    while (!all_characters_guessed)
    {
        if (wrong_guesses == MAX_GUESS_ATTEMPTS)
        {
            printf("You lost!\n");
            break;
        }

        print("%%GThe word is: %s%%0", guessed_word);
        printf("  Guess: ");
        char guess;
        scanf(" %c", &guess);
        guess = tolower(guess);

        int found = 0;
        for (int i = 0; i < strlen(word); i++)
        {
            if (tolower(word[i]) == guess)
            {
                guessed_word[i] = word[i];
                found = 1;
            }
        }

        if (!found)
        {
            wrong_guesses++;
            print("%%RWrong guess! Remaining attempts: %d%%0", (MAX_GUESS_ATTEMPTS - wrong_guesses));
            draw_hangman(wrong_guesses);
        }

        all_characters_guessed = true;
        for (int i = 0; i < strlen(word); i++)
        {
            if (guessed_word[i] == '*')
            {
                all_characters_guessed = false;
                break;
            }
        }
        print("");
    }
}

// Check if the game is over
int is_game_over(char word[], int letter_states[], int guess_count)
{
    int word_length = strlen(word);
    int correct_count = 0;
    for (int i = 0; i < word_length; i++)
    {
        if (letter_states[i])
        {
            correct_count++;
        }
    }
    if (correct_count == word_length)
    {
        printf("\nCongratulations! You guessed the word. The word is: '%s'\n", word);
        return 1;
    }

    if (guess_count == MAX_GUESSES)
    {
        draw_hangman(guess_count);
        printf("\nYou lost. The word was '%s'\n", word);
        return 1;
    }

    return 0;
}

int main(int argc, char *argv[])
{

    char word[MAX_WORD_LENGTH];
    int letter_states[MAX_WORD_LENGTH] = {0};

    int guess_count = 0;
    int word_length;

    print_logo();

    char *SERVER_IP_ADDRESS = argv[1],
         *SERVER_PORT = argv[2];
    struct sockaddr_in server_addr;

    exit_on_wrong_usage(argc, argv);

    print("%%YClient is starting...%%0");

    // server connection setup...
    // create a socket for the client
    int client_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (client_sock < 0)
    {
        exit_with_error("Error: socket() Failed.");
    }

    // set up the server address structure
    // and connect to the server
    setup_server_addr(&server_addr, SERVER_IP_ADDRESS, atoi(SERVER_PORT));
    connect_to_server(client_sock, &server_addr);

    // start of communication/game
    print("%%GSuccessfully connected to server at %s:%s.%%0", SERVER_IP_ADDRESS, SERVER_PORT);
    int current_attempts = 7;
    // init the buffers to be recieved -- category, role.
    char category[BUFF_SIZE], role[BUFF_SIZE];

    // recieve the buffer
    bzero(category, BUFF_SIZE);
    int r_category_res = recv(client_sock, category, BUFF_SIZE, 0);

    print("%%G--- Starting Game ---%%0");
    // categories is...
    print("CATEGORY: %%G%s%%0", category);

    // recieve the buffer
    bzero(role, BUFF_SIZE);
    int role_res = recv(client_sock, role, BUFF_SIZE, 0);

    // your role is...
    print("ROLE: %%G%s%%0", role);
    print("");

    char message[BUFF_SIZE];

    if (equal(role, "PROVIDER"))
    {
        // an input from the user
        printf("  Enter a word: ");
        bzero(message, BUFF_SIZE);
        fgets(message, BUFF_SIZE, stdin);

        // send to the server
        int s_message_res = send(client_sock, message, BUFF_SIZE, 0);

        // main loop
        while (guess_count < MAX_GUESS_ATTEMPTS)
        {
            // receive the message
            bzero(message, BUFF_SIZE);
            int r_message_res = recv(client_sock, message, BUFF_SIZE, 0);
            print("%%YOther player guessed: %s%%0", message);
            guess_count++;
            if (guess_count == MAX_GUESS_ATTEMPTS) // if 7 guesses have been made, break the loop
            {
                print("%%RGuesser used up all attempts.%%0");
                break;
            }
        }
    }

    if (equal(role, "GUESSER"))
    {
        print("%%YWaiting for the other player to provide a word...%%0");
        // receive the message
        bzero(message, BUFF_SIZE);
        int r_message_res = recv(client_sock, message, BUFF_SIZE, 0);

        // store the first word
        char word[BUFF_SIZE];
        strcpy(word, message);

        // mask the word
        char masked_message[BUFF_SIZE];
        hide_word(masked_message, message);

        // main loop
        guess(word);
    }

    close(client_sock);

    return 0;
}
