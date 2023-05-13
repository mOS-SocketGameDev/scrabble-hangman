#include <stdbool.h>
#ifndef FUNCTIONS_H
#define FUNCTIONS_H

void exit_with_error(char *error_msg);
void close_sockets(int c1_sock, int c2_sock, int s_sock);
bool equal(char *s1, char *s2);

#endif