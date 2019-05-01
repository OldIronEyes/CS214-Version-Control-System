#ifndef WTFClient_H
#define WTFClient_H



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


int connect_server (char * Ip, char *  port);

node *  tokenize();

void configue ( char* IPAddress, char* port );

#endif


