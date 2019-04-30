#ifndef Test_H
#define Test_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <openssl/sha.h>
#include <sys/stat.h>
#include <math.h>

int MANIFEST_ENTRIES;

static int newFlag = O_WRONLY | O_CREAT;
static int readFlag = O_RDONLY;
static int addFlag = O_RDWR | O_APPEND;
static int remFlag = O_RDWR | O_TRUNC;
static mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;

typedef struct _manEntry {
	int verNum;
	char* name;
	char* hash;
	char* text;
} manEntry;

char* generateHash(char* input);
manEntry* newManEntry(char* fileName);
manEntry* extractEntry(char* rawText, int trailer);
manEntry** readManifest(char* projectName);

void createProject(char* projectName);
void addFile(char* projectName, char* fileName);
void removeFile(char* projectName, char* fileName);
void destroyProject(char* projectName);

#endif
