#include "WTF.h"

char* parseUpdateName(char* projectName){
	char* manifestName = malloc((strlen(projectName)+8)*sizeof(char));
	memset(manifestName, 0x0, (strlen(projectName)+8));
	strncpy(manifestName, projectName, strlen(projectName));
	strncat(manifestName, ".update", 7);
	return manifestName;
}

char* genLiveHash(char* fileName){
	struct stat buffer;
	stat(fileName, &buffer);
	
	char* hash;
	
	if(buffer.st_size == 0){
		hash = generateHash(fileName);
	} else {
		int contents = open(fileName, readFlag, mode);
		char* fileText = malloc(buffer.st_size*sizeof(char) + 1);
		memset(fileText, 0x0, buffer.st_size*sizeof(char) + 1);
		read(contents, fileText, buffer.st_size);
		close(contents);
		hash = generateHash(fileText);
		free(fileText);
	}
	
	return hash;
}

void compareManifests(manEntry** client, int cEntries, manEntry** server, int sEntries){
	//Version check, positive = client ahead, 0 = same, negative = server ahead
	int version = client[0]->verNum - server[0]->verNum;
	int i, j;
	int longer;
	char* liveHash;
	int exists = 0;
	
	if(cEntries > sEntries){
		longer = cEntries;
	} else {
		longer = sEntries;
	}
	
	//Iterate through the client manifest for UMDE flags
	for(i = 0; i < cEntries; i++){
		exists = 0;
		//Ignore the entry for the manifest itself
		if(!strstr(client[i]->name, ".manifest")){
			liveHash = genLiveHash(client[i]->name);
			
			for(j = 0; j < sEntries; j++){
				//If the file is in both manifests
				if(strcmp(client[i]->name, server[j]->name) == 0){
					exists = 1;
					//Update Case
					if(version == 0 && strcmp(server[j]->hash, liveHash) != 0){
						client[i]->code = 'U';
					}
					//Modify case
					else if(version != 0 && server[j]->verNum != client[i]->verNum && strcmp(client[i]->hash, liveHash) == 0){
						client[i]->code = 'M';
					}
					else{
						client[i]->code = 'E';
					}
					break;
				}
			}
			//If the file is in the client but not the server
			if(!exists){
				if(version == 0){
					client[i]->code = 'U';
				} else {
					client[i]->code = 'D';
				}
			}
		free(liveHash);
		}
	}
	
	//Iterating through the server manifest for A flag
	for(j = 0; j < sEntries; j++){
		exists = 0;
		//Ignore entry for the manifest itself
		if(!strstr(server[j]->name, ".manifest")){
			for(i = 0; i < cEntries; i++){
				if(strcmp(client[i]->name,server[j]->name) == 0){
					exists = 1;
				}
			}
		}
		//If the file is on the server but not the client
		if(!exists){
			server[j]->code = 'A';
		} else{
			//The file has already been taken care of in the previous loop
			server[j]->code = '-';
		}
		
	}
}

void writeUpdateEntry(manEntry* entry, int fileDescriptor){
	if(entry->code != '-' || entry->code != 'E'){
		//Get full path
		char path[4097];
		memset(path, 0x0, 4097);
		char* filePath = realpath(entry->name, path);
		
		//Get length of version number and path
		int nLen = floor(log10(entry->verNum)) + 1;
		int pLen = strlen(filePath);
		
		//Space for char, version length, path length, hash, 3 commas, a newline, and null terminator
		char* output = malloc((1+nLen+pLen+40+4)*sizeof(char)+1);
		memset(output, 0x0, (1+nLen+pLen+40+4));
		
		sprintf(output, "%c,%d,%s,%s\n", entry->code, entry->verNum, filePath, entry->hash);
		printf("%s", output);
		
		if(entry->code != 'U'){
			write(fileDescriptor, output, strlen(output));
		}
		free(output);
	}
}

void outputError(manEntry** entryArray, int entries){
	int i;
	char path[4097];
	memset(path, 0x0, 4097);
	
	printf("Conflicts were detected in the following files. Please resolve before updating again.\n");
	
	for(i = 0 ; i < entries; i++){
		if(entryArray[i]->code == 'E'){
			char* filePath = realpath(entryArray[i]->name, path);
			printf("%s\n", filePath);
		}
	}
}
























