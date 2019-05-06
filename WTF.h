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

//Update functions
char* parseUpdateName(char* projectName);
void compareManifests(manEntry** client, int cEntries, manEntry** server, int sEntries);
void writeUpdateEntry(manEntry* entry, int fileDescriptor);
void outputError(manEntry** entryArray, int entries);
manEntry* extractUpdate(char* rawText, int trailer);
char** getFileNames (manEntry** updateArray, int uEntries);

#endif
