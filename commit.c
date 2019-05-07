#include "WTF.h"

char* parseCommitName(char* projectName){
	char* commitName = malloc((strlen(projectName)+8)*sizeof(char));
	memset(commitName, 0x0, (strlen(projectName)+8));
	strncpy(commitName, projectName, strlen(projectName));
	strncat(commitName, ".commit", 7);
	return commitName;
}

void compareCommitManifests(manEntry** client, int cEntries, manEntry** server, int sEntries){
	int i;
	int j;
	int exists;
	
	//Setting Upload and Error flags for client manifest
	for(i = 1; i < cEntries; i++){
		exists = 0;
		for(j = 1; j <sEntries; j++){
			if(strcmp(client[i]->name, server[j]->name) == 0 && strcmp(client[i]->hash, server[j]->hash) != 0){
				if(client[i]->verNum > server[j]->verNum){
					client[i]->code ='U';
				} else if (client[i]->verNum < server[j]->verNum){
					client[i]->code ='E';
				}
				server[i]->code = '-';
				server[j]->code = '-';
				exists = 1;
			}
		}
		if(!exists){
			client[i]->code = 'U';
		}
	}
	
	//Setting Delete flags for the server manifest
	for(j = 0; j < sEntries; j++){
		if(server[j]->code == 'N'){
			server[j]->code = 'D';
		}
	}
}

void writeCommitEntry(manEntry* entry, int fileDescriptor){
	if(entry->code != '-'){
		int textLength;
		
		int nLen = floor(log10(entry->verNum)) + 1;
		textLength = strlen(entry->name) + nLen + 5;
		
		char* entryText = malloc(textLength);
		memset(entryText, 0x0, textLength);
		
		sprintf(entryText, "%c,%d,%s\n", entry->code, entry->verNum, entry->name);
		
		write(fileDescriptor, entryText, strlen(entryText));
		
		free(entryText);
	}
}


manEntry* extractCommit(char* rawText, int trailer){
	manEntry* entry = malloc(sizeof(manEntry));
	int leader = trailer;
	
	//Get file code
	entry->code = rawText[trailer];
	while(rawText[leader] != ','){
		leader++;
	}
	trailer = leader;
	
	//Get version number
	while(rawText[leader] != ','){
		leader++;
	}
	char* version = malloc((leader-trailer)+1*sizeof(char));
	strncpy(version, (char*)rawText+trailer, (leader-trailer));
	entry->verNum = atoi(version);
	free(version);
	leader++;
	
	//Get file name
	trailer = leader;
	while(rawText[leader] != '\n'){
		leader++;
	}
	entry->name = malloc((leader-trailer)*sizeof(char)+1);
	memset(entry->name, 0x0, (leader-trailer)*sizeof(char)+1);
	strncpy(entry->name, (char*)rawText+trailer, (leader-trailer));
	
	return entry;
}

manEntry** readCommit(char* commitName){
	struct stat buffer;
	stat(commitName, &buffer);
	
	int contents = open(commitName, readFlag, mode);
	char* commitText = malloc(buffer.st_size*sizeof(char)+1);
	memset(commitText, 0x0, buffer.st_size*sizeof(char)+1);
	read(contents, commitText, buffer.st_size);
	close(contents);
	
	//Convert into array of manifest Entries
	int numEntries = 0;
	int maxEntries = 10;
	int leader = 0;
	int trailer = 0;
	
	manEntry** entryArray = malloc(maxEntries*sizeof(manEntry*));
	
	while(leader < buffer.st_size){
		while(commitText[leader]!='\n'){
			leader++;
		}
		entryArray[numEntries] = extractCommit(commitText, trailer);
		numEntries++;
		
		leader++;
		trailer=leader;
		
		if(numEntries == maxEntries){
			maxEntries *= 1.2;
			entryArray = realloc(entryArray, maxEntries*sizeof(manEntry));
		}
	}
	
	MANIFEST_ENTRIES = numEntries;
	
	return entryArray;
}
