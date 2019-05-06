#ifndef WTFClient_H
#define WTFClient_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <ctype.h>
#include "WTF.h"

void error(const char *msg);

void* func(void *vargp);

#endif
