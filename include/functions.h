#ifndef FUNCTIONS_H
#define FUNCTIONS_H

void exit_with_error(char *error_msg);
void close_sockets(int c1_sock, int c2_sock, int s_sock);

// Communications
void send_to(int client_sock, char buffer[]);
void recv_from(int client_sock, char buffer[]);

#endif