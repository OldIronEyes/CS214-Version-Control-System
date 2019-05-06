#ifndef WTF_H
#define WTF_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <openssl/sha.h>
#include <sys/stat.h>
#include <math.h>
#include <netdb.h> 
#include <sys/socket.h> 
#include <sys/types.h>
#include <netinet/in.h>
#include <ctype.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>


int MANIFEST_ENTRIES;

char* IP;
int PORT;

static int newFlag = O_WRONLY | O_CREAT;
static int readFlag = O_RDONLY;
static int addFlag = O_RDWR | O_APPEND;
static int remFlag = O_RDWR | O_TRUNC;
static mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;

typedef struct _manEntry {
	int verNum;
	char* name;
	char* hash;
	char code;
} manEntry;


typedef struct node{
    char * data;
    struct node*next;
} node;


//Basic Manifest Functions
char* parseManifestName(char* projectName);
char* generateHash(char* inputText);
manEntry* newManEntry(char* fileName);
void updateManEntry(manEntry* entry);
void writeManEntry(manEntry* entry, int fileDescriptor);
manEntry* extractEntry(char* rawText, int trailer);
manEntry** readManifest(char* projectName);
void freeManifest(manEntry** manifest);
void freeManEntry(manEntry* entry);

//Core client functions
void createProject(char* projectName);
void addFile(char* projectName, char* fileName);
void removeFile(char* projectName, char* fileName);
void destroyProject(char* projectName);
void updateProject(char* projectName);

//connection to server functions 
void configure ( char* IPAddress, char* port );
void tokenize();
int send_file(char * path, int socket);
int connect_server (char * Ip, int port);






//Update functions
void compareManifests(manEntry** client, int cEntries, manEntry** server, int sEntries);

#endif
