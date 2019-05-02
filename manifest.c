#include "WTF.h"

char* parseManifestName(char* projectName){
	char* manifestName = malloc((strlen(projectName)+10)*sizeof(char));
	memset(manifestName, 0x0, (strlen(projectName)+10));
	strncpy(manifestName, projectName, strlen(projectName));
	strncat(manifestName, ".manifest", 10);
	return manifestName;
}

char* generateHash(char* inputText){
	int i;
	char temp[SHA_DIGEST_LENGTH];
	char* buff = malloc((SHA_DIGEST_LENGTH*2)*sizeof(char));
	
	memset(temp, 0x0, SHA_DIGEST_LENGTH);
	memset(buff, 0x0, SHA_DIGEST_LENGTH*2);
	
	SHA1((const char *)inputText, (unsigned long)strlen(inputText), temp);
	
	for(i = 0; i <SHA_DIGEST_LENGTH; i++){
		sprintf((char*)&(buff[i*2]), "%02x", temp[i]);
	}
	return buff;
}

manEntry* newManEntry(char* fileName){
	struct stat buffer;
	stat(fileName, &buffer);
	
	manEntry* project = malloc(sizeof(manEntry));
	project->verNum = 1;
	project->name = malloc(strlen(fileName)*sizeof(char)+1);
	strncpy(project->name, fileName, strlen(fileName));
	
	int nLen = floor(log10(project->verNum))+1;
	
	int contents = open(fileName, readFlag, mode);
	char* fileText = malloc(buffer.st_size*sizeof(char)+1);
	read(contents, fileText, buffer.st_size);
	close(contents);
	
	project->hash = generateHash(fileText);
	free(fileText);
	
	project->text = malloc(nLen+strlen(project->name)+25);
	
	sprintf(project->text, "%d,%s,%s\n", project->verNum,project->name,project->hash);
	return project;
}

void updateManEntry(manEntry* entry){
	entry->verNum++;
	int nLen = floor(log10(entry->verNum))+1;
	entry->text = realloc(entry->text,(nLen+strlen(entry->name)+25));
	
	struct stat buffer;
	stat(entry->name, &buffer);
	
	int contents = open(entry->name, readFlag, mode);
	char* fileText = malloc(buffer.st_size*sizeof(char)+1);
	read(contents, fileText, buffer.st_size);
	close(contents);
	
	entry->hash = generateHash(fileText);
	free(fileText);
	
	sprintf(entry->text, "%d,%s,%s\n", entry->verNum,entry->name,entry->hash);
}

manEntry* extractEntry(char* rawText, int trailer){
	manEntry* entry = malloc(sizeof(manEntry));
	
	//Get the version number
	int leader = trailer;
	while(rawText[leader] != ','){
		leader++;
	}
	char* text = malloc((leader-trailer+1)*sizeof(char));
	strncpy(text,(char*)rawText+trailer,(leader-trailer));
	entry->verNum = atoi(text);
	free(text);
	leader++;
	
	//Get the file name
	trailer = leader;
	while(rawText[leader] != ','){
		leader++;
	}
	entry->name = malloc((leader-trailer)*sizeof(char));
	strncpy(entry->name,(char*)rawText+trailer,(leader-trailer));
	leader++;
	
	entry->hash = malloc(21*sizeof(char));
	strncpy(entry->hash, (char*)rawText+trailer,20);
	
	return entry;
}

manEntry** readManifest(char* projectName){
	//Get size of manifest file
	struct stat buffer;
	stat(projectName, &buffer);
	
	//Read manifest into memory
	int contents = open(projectName, readFlag, mode);
	char* manText = malloc(buffer.st_size*sizeof(char)+1);
	read(contents, manText, buffer.st_size);
	close(contents);
	
	//Convert into array of manifest Entries
	int numEntries = 0;
	int maxEntries = 10;
	int leader = 0;
	int trailer = 0;
	manEntry** entryArray = malloc(maxEntries*sizeof(manEntry*));
	
	while(manText[leader]!='\0'){
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
