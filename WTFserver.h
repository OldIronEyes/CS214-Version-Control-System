#ifndef WTFClient_H
#define WTFClient_H



#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void error(const char *msg);

void* func(void *vargp);

int main(int argc, char *argv[]);



#endif