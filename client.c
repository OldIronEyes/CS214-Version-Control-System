#include "WTF.h"

void createProject(char* projectName){
	int contents = open(projectName, newFlag, mode);
	manEntry* projectEntry = newManEntry(projectName);
	write(contents, projectEntry->text, strlen(projectEntry->text));
	close(contents);
}

void destroyProject(char* projectName){
	remove(projectName);
	
	//TODO send command to server to remove project files
}

void addFile(char* projectName, char* fileName){
	manEntry** manifest = readManifest(projectName);
	int i;
	
	for(i = 0; i < MANIFEST_ENTRIES; i++){
		if(strcmp(manifest[i]->name,fileName) == 0);
		printf("This file is already being tracked");
		exit;
	}
	
	int contents = open(projectName, addFlag, mode);
	manEntry* newFile = newManEntry(fileName);
	write(contents, newFile->text, strlen(newFile->text));
	close(contents);
	free(newFile);
}

void removeFile(char* projectName, char* fileName){
	int i;
	int contents = open(projectName, remFlag, mode);
	manEntry** entryArray = readManifest(projectName);
	
	updateManEntry(entryArray[0]);
	
	for(i = 0; i < MANIFEST_ENTRIES; i++){
		if(strcmp(entryArray[i]->name,fileName) != 0){
			write(contents, entryArray[i]->text, strlen(entryArray[i]->text));
		}
	}
	close(contents);
}


















