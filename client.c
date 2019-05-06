#include "WTF.h"

void createProject(char* projectName){
	//TODO check if the project already exists on the server
	
	char* manifestName = parseManifestName(projectName);
	int contents = open(manifestName, newFlag, mode);
	manEntry* newProject = newManEntry(manifestName);
	writeManEntry(newProject, contents);
	close(contents);
	free(manifestName);
	freeManEntry(newProject);
}

void destroyProject(char* projectName){
	char* manifestName = parseManifestName(projectName);
	remove(manifestName);
	free(manifestName);
	
	//TODO send command to server to remove project files
}

void addFile(char* projectName, char* fileName){
	char* manifestName = parseManifestName(projectName);
	manEntry** manifest = readManifest(manifestName);
	int i;
	
	for(i = 0; i < MANIFEST_ENTRIES; i++){
		if(strcmp(manifest[i]->name,fileName) == 0){
			printf("This file is already being tracked\n");
			freeManifest(manifest);
			exit(0);
		}
	}
	
	manEntry* newFile = newManEntry(fileName);
	
	int contents = open(manifestName, addFlag, mode);
	writeManEntry(newFile, contents);
	close(contents);
	freeManEntry(newFile);
	freeManifest(manifest);
	free(manifest);
}

void removeFile(char* projectName, char* fileName){
	int i;
	char* manifestName = parseManifestName(projectName);
	
	if(strcmp(manifestName, fileName) == 0){
		printf("Can't stop tracking .manifest");
		exit(0);
	}
	
	//Check if file is being tracked
	struct stat buffer;
	stat(manifestName, &buffer);
	int contents = open(manifestName, readFlag, mode);
	char* manText = malloc(buffer.st_size*sizeof(char)+1);
	read(contents, manText, buffer.st_size);
	close(contents);
	if(strstr(manText, fileName) == 0){
		printf("This file is not being tracked\n");
		exit(0);
	}
	
	//Read manifest and remove file
	manEntry** entryArray = readManifest(manifestName);
	
	contents = open(manifestName, remFlag, mode);
	
	for(i = 0; i < MANIFEST_ENTRIES; i++){
		if(strcmp(entryArray[i]->name,fileName) != 0){
			writeManEntry(entryArray[i], contents);
		}
	}
	close(contents);
}

void checkoutProject(char* projectName){
	//TODO: Send command, recieve entire project, decompress it
}

void updateProject(char* projectName){
	//TODO: Send command, recieve server's .manifest file
	
	char* manifestName = parseManifestName(projectName);
	manEntry** clientManifest = readManifest(manifestName);
	int cEntries = MANIFEST_ENTRIES;
	
	//Pointer should be to .server's manifest text
	manEntry** serverManifest = readManifest("ay2.manifest");
	int sEntries = MANIFEST_ENTRIES;
	
	compareManifests(clientManifest, cEntries, serverManifest, sEntries);
}



























