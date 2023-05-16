#include <stdbool.h>
#ifndef FUNCTIONS_H
#define FUNCTIONS_H

void exit_with_error(char *error_msg);
void close_sockets(int c1_sock, int s_sock);
bool equal(char *s1, char *s2);

// client/server connection logic
void setup_server_addr(struct sockaddr_in *server_addr, const char *ip_address, int port);
void connect_to_server(int client_sock, struct sockaddr_in *server_addr);
void bind_to_server(int server_sock, int port);
int accept_client(int server_sock, struct sockaddr_in *client_addr);

void print(const char *format, ...);
void print_logo();

void draw_hangman(int guess_count);
void hide_word(char masked_message[], char word[]);
bool guess_handler(char word[]);
#endif