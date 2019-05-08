#include "WTF.h"

void createProject(char* projectName){
	int length = 7;
	int err = 0;
	
	while(write(SERVER,&length, sizeof(int)) < sizeof(int));
	while(write(SERVER,"create", 7) < 7);
	length = strlen(projectName)+1;
	while(write(SERVER, &length, sizeof(int)) < sizeof(int));
	
	
	while(write(SERVER, &projectName, strlen(projectName)) < strlen(projectName));

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
	
	int length = 8;
	while(write(SERVER, &length, sizeof(int)) < sizeof(int));
	while(write(SERVER, "destroy", 8) < 8);
	
	length = strlen(projectName)+1;
	while(write(SERVER, &length, sizeof(int)) < sizeof(int));
	while(write(SERVER, projectName, strlen(projectName)) < strlen(projectName));
	
	close(SERVER);
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
	free(manifestName);
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

void updateProject(char* projectName){
	int buffer = 6;
	while(write(SERVER, &buffer, sizeof(int)) < sizeof(int));
	while(write(SERVER, "update", 6) < 6);
	
	while(read(SERVER, &buffer, sizeof(int)) < sizeof(int));
	char* serverManifestText = malloc((buffer+1)*sizeof(char));
	while(read(SERVER, serverManifestText, buffer) < buffer);
	
	close(SERVER);
	
	int sManFile = open("a2.manifest", newFlag, 0644);
	write(sManFile, serverManifestText, strlen(serverManifestText));
	close(sManFile);

	int i;
	int error = 0;
	
	char* manifestName = parseManifestName(projectName);
	manEntry** clientManifest = readManifest(manifestName);
	int cEntries = MANIFEST_ENTRIES;
	
	//Pointer should be to .server's manifest text
	manEntry** serverManifest = readManifest("a2.manifest");
	remove("a2.manifest");
	int sEntries = MANIFEST_ENTRIES;
	
	compareUpdateManifests(clientManifest, cEntries, serverManifest, sEntries);
	
	for(i = 0; i < cEntries; i++){
		if(clientManifest[i]->code == 'E'){
			error = 1;
			break;
		}
	}
	
	if(error){
		outputError(clientManifest, cEntries);
	} else {
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
	char* updateName = parseUpdateName(projectName);
	
	if(access(updateName, F_OK) == -1){
		printf("Update file does not exist, run Update <Project Name> first");
		close(SERVER);
		exit(0);
	}
	
	struct stat updateFile;
	stat(updateName, &updateFile);
	if(updateFile.st_size == 0){
		printf("Update file is blank, no Upgrade required\n");
		close(SERVER);
		exit(0);
	}
	
	int buffer = 8;
	while(write(SERVER, &buffer, sizeof(int)) < sizeof(int));
	while(write(SERVER, "upgrade", 8) < 8);
	
	buffer = strlen(projectName)+1;
	while(write(SERVER, &buffer, sizeof(int)) < sizeof(int));
	while(write(SERVER, projectName, strlen(projectName)) < strlen(projectName));
	
	manEntry** updateArray = readUpdate(updateName);
	int uEntries = MANIFEST_ENTRIES;
	
	char** fileNames = getFileNames(updateArray, uEntries);
	
	
	//Tar File Name 
	while(read(SERVER,&buffer,sizeof(int)) < sizeof(int));
	char* tarName = malloc(buffer*sizeof(char)+1);
	while(read(SERVER, tarName, buffer) < buffer);
	

	//Tar Contents
	while(read(SERVER, &buffer, sizeof(int)) < sizeof(int));
	char* tarContents= malloc(buffer*sizeof(char)+1);
	while(read(SERVER, tarContents, buffer) < buffer);
	

	//Write Tar to disk
	int contents = open(tarName, newFlag, mode);
	write(contents, tarContents, buffer);
	close(contents);

	mainExtract(tarName);
}

void commitProject(char* projectName){
	int noUpdate = 0;
	char* updateName = parseUpdateName(projectName);
	struct stat file;
	if(access(updateName, F_OK) == 0){
		
		stat(updateName, &file);
		if(file.st_size != 0){
			noUpdate = 1;
		}
	}
	free(updateName);
	
	if(noUpdate){
		printf("Your project is behind the repository. Update and Upgrade to sync up before attempting to commit again\n");
		exit(0);
	}
	
	int buffer = 7;
	while(write(SERVER, &buffer, sizeof(int)) < sizeof(int));
	while(write(SERVER, "commit", 7) < 7);
	
	
	//Project name
	buffer = strlen(projectName)+1;
	while(write(SERVER, &buffer, sizeof(int)) < sizeof(int));
	while(write(SERVER, projectName, strlen(projectName)) < strlen(projectName));
	

	//Get Server Manifest
	while(read(SERVER, &buffer, sizeof(int)) < sizeof(int));
	char* manifestText = malloc(buffer*sizeof(char)+1);
	while(read(SERVER, manifestText, buffer) < buffer);
	

	int servermanifest = open("a2.manifest", newFlag, mode);
	write(servermanifest, manifestText, strlen(manifestText));
	close(servermanifest);

	int i;
	int error = 0;
	
	char* manifestName = parseManifestName(projectName);
	manEntry** clientManifest = readManifest(manifestName);
	int cEntries = MANIFEST_ENTRIES;
	char* liveHash;
	
	for(i = 0; i < cEntries; i++){
		liveHash = genLiveHash(clientManifest[i]->name);
		if(strcmp(liveHash, clientManifest[i]->hash) != 0){
			clientManifest[i]->verNum++;
			free(liveHash);
		}
	}
	
	
	manEntry** serverManifest = readManifest("a2.manifest");
	remove("a2.manifest");
	int sEntries = MANIFEST_ENTRIES;
	for(i = 0; i < sEntries; i++){
		serverManifest[i]->code = 'N';
	}
	
	if(clientManifest[0]->verNum != serverManifest[0]->verNum){
		printf("The version numbers of your projects do not match.\n");
		printf("Update and upgrade your local files first.\n");
		exit(0);
	}
	
	compareCommitManifests(clientManifest, cEntries, serverManifest, sEntries);
	
	for(i = 0; i < cEntries; i++){
		if(clientManifest[i]->code == 'E'){
			error = 1;
			break;
		}
	}
	if(error){
		printf("Error in project file versions, update and upgrade local files\n");
		exit(0);
	}
	
	for(i = 0; i < cEntries; i++){
		if(clientManifest[i]->code == '-'){
		error++;
		}
	}
	for(i = 0; i < sEntries; i++){
		if(serverManifest[i]->code == '-'){
		error++;
		}
	}
	
	if(error == (cEntries + sEntries)){
		printf("No files have modified, added, or deleted. No commit necessary\n");
		exit(0);
	}
	
	char* commitName = parseCommitName(projectName);
	int contents = open(commitName, newFlag, mode);
	
	for(i = 0; i < cEntries; i++){
		writeCommitEntry(clientManifest[i], contents);
	}
	for(i = 0; i < sEntries; i++){
		writeCommitEntry(serverManifest[i], contents);
	}
	close(contents);
	sendFile(commitName);
}

void pushProject(char * projectName){
	char* commitName = parseCommitName(projectName);
	if(access(commitName, F_OK) != 0){
		printf("There is no commit file, commit first\n");
		exit(0);
	}
	
	int buffer = 5;
	while(write(SERVER, &buffer, sizeof(int)) < sizeof(int));
	while(write(SERVER,"push", 5) < 5);
	
	buffer = strlen(projectName)+1;
	while(write(SERVER, &buffer, sizeof(int)) < sizeof(int));
	while(write(SERVER, projectName, strlen(projectName)) < strlen(projectName));
	
	
	manEntry** commitArray = readCommit(commitName);
	int cEntries = MANIFEST_ENTRIES;
	
	char* tarName = mainCompress(projectName, commitArray, cEntries);
	
	sendFile(tarName);
	
	remove(commitName);
	remove(tarName);
	free(tarName);
}

int sendFile(char* path){
	int contents = open(path, O_RDONLY);
	int length = lseek(contents, 0, SEEK_END);
	lseek(contents, 0, SEEK_SET);
	
	char* fileText = malloc(length*sizeof(char)+1);
	write(contents, fileText, length);
	lseek(contents, 0, SEEK_SET);
	
	while(write(SERVER, &length ,sizeof(int)) < sizeof(int));
	while(write(SERVER, fileText, length) < length);
	
	close(contents);
	}
