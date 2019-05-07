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
	
	int i;
	int error = 0;
	
	char* manifestName = parseManifestName(projectName);
	manEntry** clientManifest = readManifest(manifestName);
	int cEntries = MANIFEST_ENTRIES;
	
	//Pointer should be to .server's manifest text
	manEntry** serverManifest = readManifest("a2.manifest");
	int sEntries = MANIFEST_ENTRIES;
	
	//compareUpdateManifests(clientManifest, cEntries, serverManifest, sEntries);
	
	//Check for E flags
	for(i = 0; i < cEntries; i++){
		if(clientManifest[i]->code == 'E'){
			error = 1;
			break;
		}
	}
	
	if(error){
		outputError(clientManifest, cEntries);
	} else {
		//Write update codes
		char* updateName = parseUpdateName(projectName);
		int contents = open(updateName, newFlag, mode);
		for(i = 0; i < cEntries; i++){
			writeUpdateEntry(clientManifest[i], contents);
		}
		for(i = 0; i < sEntries; i++){
			writeUpdateEntry(serverManifest[i], contents);
		}
		close(contents);
	}
	exit(0);
}

void upgradeProject(char* projectName){
	//TODO Check if project is on the server
	
	char* updateName = parseUpdateName(projectName);
	
	if(access(updateName, F_OK) == -1){
		printf("Update file does not exist, run Update <Project Name> first");
		exit(0);
	}
	
	struct stat buffer;
	stat(updateName, &buffer);
	if(buffer.st_size == 0){
		printf("Update file is blank, no Upgrade required\n");
		
	}
	
	manEntry** updateArray = readUpdate(updateName);
	int uEntries = MANIFEST_ENTRIES;
	
	char** fileNames = getFileNames(updateArray, uEntries);
	
	//TODO Request files from server
	
	//TODO Write files to the paths in updateArray[i]->name
}

//TODO
//TODO COMMIT NEEDS TO BE FINISHED
//TODO
void commitProject(char* projectName){
	//TODO Check for .update file
	
	//TODO Check if project exists on the server
	
	//TODO Recieve server's .Manifest file
	
	int i;
	int error = 0;
	
	char* manifestName = parseManifestName(projectName);
	manEntry** clientManifest = readManifest(manifestName);
	int cEntries = MANIFEST_ENTRIES;
	
	//Pointer should be to server's .manifest text
	manEntry** serverManifest = readManifest("a2.manifest");
	int sEntries = MANIFEST_ENTRIES;
	
	if(clientManifest[0]->verNum != serverManifest[0]->verNum){
		printf("The version numbers of your projects do not match.\n");
		printf("Update and upgrade your local files first.\n");
		exit(0);
	}
	
	//compareCommitManifests(clientManifest, cEntries, serverManifest, sEntries);
	
	for(i = 0; i < cEntries; i++){
		if(clientManifest[i]->code == 'E'){
			error = 1;
			break;
		}
	}
	if(error){
		outputError(clientManifest, cEntries);
	}
}

























