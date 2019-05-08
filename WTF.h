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
#include <dirent.h>
#include <ctype.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <dirent.h>
#include <errno.h>

int MANIFEST_ENTRIES;

char* IP;
int PORT;

int SERVER;

static int newFlag = O_WRONLY | O_CREAT;
static int readFlag = O_RDONLY;
static int addFlag = O_RDWR | O_APPEND;
static int remFlag = O_RDWR | O_TRUNC;
static mode_t mode = S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IROTH;

typedef struct _manEntry {
	int verNum;
	char* name;
	char* hash;
	char code;
} manEntry;

typedef struct node{
    char* data;
    struct node* next;
} node;

//Core client Functions
void createProject(char* projectName);
void addFile(char* projectName, char* fileName);
void removeFile(char* projectName, char* fileName);
void destroyProject(char* projectName);
void updateProject(char* projectName);
void upgradeProject(char* projectName);
void commitProject(char* projectName);
void pushProject(char* projectName);

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

//Update+Upgrade Functions
char* parseUpdateName(char* projectName);
void compareUpdateManifests(manEntry** client, int cEntries, manEntry** server, int sEntries);
void writeUpdateEntry(manEntry* entry, int fileDescriptor);
manEntry* extractUpdate(char* rawText, int trailer);
void outputError(manEntry** entryArray, int entries);
char** getFileNames (manEntry** updateArray, int uEntries);

//Commit+Push Functions
char* parseCommitName(char* projectName);
void compareCommitManifests(manEntry** client, int cEntries, manEntry** server, int sEntries);
void writeCommitEntry(manEntry* entry, int fileDescriptor);
manEntry* extractCommit(char* rawText, int trailer);
manEntry** readCommit(char* commitName);

//Compression Functions
/////mainCompress is to be used on the client side and assumes input from a .Commit file
char* mainCompress(char* projectName, manEntry** entries, int cEntries);
char* serverCompress(char* projectName);
/////mainExtract is to be used on the client side after recieving the project files
/////then has to remove() the archive after extracting files
void mainExtract(char* tarName);
/////serverExtract assumes that the archive is already in the /<projectName>/ folder
void serverExtract(char* projectName, char* tarPath);

// functions for the connection 
void connect_server (char * Ip, int port);
void readConfigure();
void configure ( char* IPAddress, char* port );

//Server functions 
void serverCommit(int fd);
void serverPush(int fd);
void serverDestroy(int fd);
void serverCreate(int fd);
void serverUpgrade(int fd);


#endif
