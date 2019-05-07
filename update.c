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

void compareUpdateManifests(manEntry** client, int cEntries, manEntry** server, int sEntries){
	//Version check, positive = client ahead, 0 = same, negative = server ahead
	int version = client[0]->verNum - server[0]->verNum;
	int i, j;
	char* liveHash;
	int exists = 0;
	
	//Iterate through the client manifest for UMDE flags
	for(i = 0; i < cEntries; i++){
		exists = 0;
		//Ignore the entry for the manifest itself
		if(strstr(client[i]->name, ".manifest") == 0){
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
					else if(version != 0 && server[j]->verNum != client[i]->verNum && strcmp(client[i]->hash, liveHash) != 0){
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
		if(strstr(server[j]->name, ".manifest") == 0){
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
	
	//Setting the manifest codes to avoid bugs
	client[0]->code = '-';
	server[0]->code = '-';
}

void writeUpdateEntry(manEntry* entry, int fileDescriptor){
	if(entry->code != '-' && entry->code != 'E'){
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

manEntry** readUpdate(char* updateName){
	struct stat buffer;
	stat(updateName, &buffer);
	int contents = open(updateName, readFlag, mode);
	char* fileText = malloc(buffer.st_size*sizeof(char) + 1);
	memset(fileText, 0x0, buffer.st_size*sizeof(char) + 1);
	read(contents, fileText, buffer.st_size);
	close(contents);
	
	int numEntries = 0;
	int maxEntries = 10;
	int leader = 0;
	int trailer = 0;
	
	manEntry** updateArray = malloc(maxEntries*sizeof(manEntry));
	
	while(leader < buffer.st_size){
		while(fileText[leader]!='\n'){
			leader++;
		}
		updateArray[numEntries] = extractUpdate(fileText, trailer);
		numEntries++;
		
		leader++;
		trailer = leader;
		
		if(numEntries == maxEntries){
			maxEntries *= 1.2;
			updateArray = realloc(updateArray, maxEntries*sizeof(manEntry));
		}
	}
	
	MANIFEST_ENTRIES = numEntries;
	
	return updateArray;
}

manEntry* extractUpdate(char* rawText, int trailer){
	manEntry* entry = malloc(sizeof(manEntry));
	
	//Get Code
	entry->code = rawText[trailer];
	
	//Get Version Number
	int leader = trailer;
	while(rawText[leader]!= ','){
		leader++;
	}
	trailer = leader;
	while(rawText[leader]!= ','){
		leader++;
	}
	char* version = malloc((leader-trailer+1)*sizeof(char));
	strncpy(version, (char*)rawText+trailer, (leader-trailer));
	entry->verNum = atoi(version);
	free(version);
	leader++;
	
	//Get the file path
	trailer = leader;
	while(rawText[leader] != ','){
		leader++;
	}
	entry->name = malloc((leader-trailer)*sizeof(char)+1);
	memset(entry->name, 0x0, (leader-trailer)*sizeof(char)+1);
	strncpy(entry->name, (char*)rawText+trailer, (leader-trailer));
	leader++;
	
	//Get the file hash
	trailer = leader;
	while(rawText[leader] != '\n' ||rawText[leader] != '\0'){
		leader++;
	}
	entry->hash = malloc(SHA_DIGEST_LENGTH*2+1);
	memset(entry->hash, 0x0, (leader-trailer)*sizeof(char));
	strncpy(entry->hash,(char*)rawText+trailer,(leader-trailer));
	
	return entry;
}

char** getFileNames (manEntry** updateArray, int uEntries){
	char** fileNameArray = malloc(uEntries*sizeof(char*));
	int i;
	int leader;
	int trailer;
	for(i = 0; i < uEntries; i++){
		leader = strlen(updateArray[i]->name);
		trailer = leader;
		while(updateArray[i]->name[trailer] != '/'){
			trailer--;
		}
		trailer++;
		fileNameArray[i] = malloc((leader-trailer)*sizeof(char)+1);
		memset(fileNameArray[i], 0x0, (leader-trailer)*sizeof(char)+1);
		strncpy(fileNameArray[i], (char*)updateArray[i]->name+trailer, (leader-trailer));
	}
	
	return fileNameArray;
}


















