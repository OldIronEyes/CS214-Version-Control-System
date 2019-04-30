#ifndef WTFConnection_from_client_H
#define WTFConnection_from_client_H



#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct node{
    char * data;
    struct node*next;
} node;


void error(const char *msg);

int connect_to_server(char* IPAddress, int port);

void tokenize();

void configue ( char* IPAddress, char* port );

#endif


