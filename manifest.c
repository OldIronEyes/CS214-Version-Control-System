#include "WTF.h"

char* parseManifestName(char* projectName){
	char* manifestName = malloc((strlen(projectName)+10)*sizeof(char));
	memset(manifestName, 0x0, (strlen(projectName)+10));
	strncpy(manifestName, projectName, strlen(projectName));
	strncat(manifestName, ".manifest", 9);
	return manifestName;
}

char* generateHash(char* inputText){
	int i;
	unsigned char temp[SHA_DIGEST_LENGTH];
	char* hash = malloc(SHA_DIGEST_LENGTH*2+1);
	
	memset(temp, 0x0, SHA_DIGEST_LENGTH);
	memset(hash, 0x0, SHA_DIGEST_LENGTH*2+1);
	SHA1((const char *)inputText, (unsigned long)strlen(inputText), temp);
	
	for(i = 0; i < SHA_DIGEST_LENGTH; i++){
		sprintf((char*)&(hash[i*2]), "%02x", temp[i]);
	}
	return hash;
}

manEntry* newManEntry(char* fileName){
	//Initialize struct
	manEntry* project = malloc(sizeof(manEntry));
	
	//Set version number
	project->verNum = 1;
	int nLen = floor(log10(project->verNum)) + 1;
	
	//Save file name
	project->name = malloc(strlen(fileName)*sizeof(char) + 1);
	memset(project->name, 0x0, strlen(fileName)*sizeof(char) + 1);
	strncpy(project->name, fileName, strlen(fileName));
	
	//Read file and generate hash
	struct stat buffer;
	stat(fileName, &buffer);
	if(buffer.st_size == 0){
		project->hash = generateHash(fileName);
	} else {
		int contents = open(fileName, readFlag, mode);
		char* fileText = malloc(buffer.st_size*sizeof(char) + 1);
		memset(fileText, 0x0, buffer.st_size*sizeof(char) + 1);
		read(contents, fileText, buffer.st_size);
		close(contents);
		project->hash = generateHash(fileText);
		free(fileText);
	}
	
	return project;
}

void updateManEntry(manEntry* entry){
	//Read file and generate hash
	struct stat buffer;
	stat(entry->name, &buffer);
	int contents = open(entry->name, readFlag);
	char* fileText = malloc(buffer.st_size*sizeof(char)+1);
	memset(fileText, 0x0, buffer.st_size*sizeof(char)+1);
	read(contents, fileText, buffer.st_size);
	close(contents);
	char* temp = generateHash(fileText);
	free(fileText);
	
	//Check if hashes match
	if(strcmp(entry->hash,temp) == 0){
		free(temp);
		exit(0);
	} else {
		//Increase version number and update hash
		entry->verNum++;
		entry->hash = temp;
		exit(0);
	}
}

void writeManEntry(manEntry* entry, int fileDescriptor){
	int verLen = floor(log10(entry->verNum))+1;
	char* entryText = malloc((strlen(entry->name)+verLen+40+4)*sizeof(char));
	sprintf(entryText, "%d,%s,%s\n", entry->verNum, entry->name,entry->hash);
	write(fileDescriptor, entryText, strlen(entryText));
	free(entryText);
}

manEntry* extractEntry(char* rawText, int trailer){
	manEntry* entry = malloc(sizeof(manEntry));
	
	//Get the version number
	int leader = trailer;
	while(rawText[leader] != ','){
		leader++;
	}
	char* version = malloc((leader-trailer+1)*sizeof(char));
	strncpy(version,(char*)rawText+trailer,(leader-trailer));
	entry->verNum = atoi(version);
	free(version);
	leader++;
	
	//Get the file name
	trailer = leader;
	while(rawText[leader] != ','){
		leader++;
	}
	entry->name = malloc((leader-trailer)*sizeof(char)+1);
	memset(entry->name, 0x0, (leader-trailer)*sizeof(char)+1);
	strncpy(entry->name,(char*)rawText+trailer,(leader-trailer));
	leader++;
	
	//Get the file hash
	trailer = leader;
	while(rawText[leader] != '\n'){
		leader++;
	}
	entry->hash = malloc(SHA_DIGEST_LENGTH*2+1);
	memset(entry->hash, 0x0, (leader-trailer)*sizeof(char));
	strncpy(entry->hash,(char*)rawText+trailer,(leader-trailer));
	
	return entry;
}

manEntry** readManifest(char* projectName){
	struct stat buffer;
	stat(projectName, &buffer);
	
	//Read manifest into memory
	int contents = open(projectName, readFlag, mode);
	char* manText = malloc(buffer.st_size*sizeof(char)+1);
	memset(manText, 0x0, buffer.st_size*sizeof(char)+1);
	read(contents, manText, buffer.st_size);
	close(contents);
	
	//Convert into array of manifest Entries
	int numEntries = 0;
	int maxEntries = 10;
	int leader = 0;
	int trailer = 0;
	
	manEntry** entryArray = malloc(maxEntries*sizeof(manEntry*));
	
	while(leader < buffer.st_size){
		while(manText[leader]!='\n'){
			leader++;
		}
		entryArray[numEntries] = extractEntry(manText, trailer);
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

void freeManifest(manEntry** manifest){
	int i;
	for(i = 0; i < MANIFEST_ENTRIES; i++){
		freeManEntry(manifest[i]);
	}
}

void freeManEntry(manEntry* entry){
	free(entry->name);
	free(entry->hash);
	free(entry);
}
