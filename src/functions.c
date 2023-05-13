#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <pthread.h>

#include "../include/functions.h"

void exit_with_error(char *error_msg)
{
    printf("%s\n", error_msg);
    exit(1);
}

void close_sockets(int c1_sock, int c2_sock, int s_sock)
{
    close(c1_sock);
    close(c2_sock);
    close(s_sock);
}

bool equal(char *s1, char *s2)
{
    return (strcmp(s1, s2) == 0);
}