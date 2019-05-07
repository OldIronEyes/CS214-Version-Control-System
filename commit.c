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

}
