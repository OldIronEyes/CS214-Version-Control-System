#include "WTF.h"

void createProject(char* projectName){
	int buffer;
	int n;
	write(SERVER,"create", 7);

	buffer = strlen(projectName)+1;
	n = write(SERVER, &buffer,sizeof(int));
	printf("n %d\n", n);
	printf("er %d\n", errno);
	n = write(SERVER,projectName,strlen(projectName));
	printf("n %d\n", n);
	printf("er %d\n", errno);

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

	write(SERVER, "destroy", 8);

	int buffer = strlen(projectName)+1;
	write(SERVER,&buffer,sizeof(int));
	write(SERVER,projectName,strlen(projectName));

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

void checkoutProject(char* projectName){
	//TODO: Send command, recieve entire project, decompress it
}

void updateProject(char* projectName){
	write(SERVER, "update",6);
	
	int length = sizeof(int);
	read(SERVER, &length, sizeof(int));
	
	char* serverManifestText = malloc((length+1)*sizeof(char));
	memset(serverManifestText, 0x0, length+1);
	read(SERVER,serverManifestText,length);
	int server_manifest = open("a2.manifest",newFlag,0644);
	write(server_manifest,serverManifestText,strlen(serverManifestText));
	close(server_manifest);

	int i;
	int error = 0;
	
	char* manifestName = parseManifestName(projectName);
	manEntry** clientManifest = readManifest(manifestName);
	int cEntries = MANIFEST_ENTRIES;
	
	//Pointer should be to .server's manifest text
	manEntry** serverManifest = readManifest("a2.manifest");
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

	int buffer;
	write(SERVER, "upgrade",8);
	buffer = strlen(projectName)+1;
	write(SERVER,&buffer,sizeof(int));
	write(SERVER,projectName,strlen(projectName));

	
	if(access(updateName, F_OK) == -1){
		printf("Update file does not exist, run Update <Project Name> first");
		exit(0);
	}
	
	struct stat third;
	stat(updateName, &third);
	if(third.st_size == 0){
		printf("Update file is blank, no Upgrade required\n");
		
	}
	
	manEntry** updateArray = (manEntry**)readUpdate(updateName);
	int uEntries = MANIFEST_ENTRIES;
	
	char** fileNames = getFileNames(updateArray, uEntries);
	
	//TODO Request files from server
	
	// get the file name
	int second;
	read (SERVER,&second,sizeof(int));
	char * file_name = malloc(second*sizeof(char));
	read(SERVER,file_name,second);

	// get the file size
	read(SERVER, &second, sizeof(int));
	char * upgraded_file= malloc(second*sizeof(char));
	read(SERVER, upgraded_file,second);

	int new_file = open(file_name,newFlag);
	write(new_file,upgraded_file,second);
	close(new_file);

	mainExtract(file_name);
}

void commitProject(char* projectName){
	int noUpdate = 1;
	char* updateName = parseUpdateName(projectName);
	struct stat file;
	if(access(updateName, F_OK) == 0){
		
		stat(updateName, &file);
		
		if(file.st_size != 0){
			noUpdate = 0;
		}
	}
	free(updateName);
	
	if(!noUpdate){
		printf("Your project is behind the repository. Update and Upgrade to sync up before attempting to commit again\n");
		exit(0);
	}
	
	write(SERVER, "commit",7);

	//Size of project name
	int buffer = strlen(projectName)+1;
	write(SERVER,&buffer,sizeof(int));
	
	//Project name
	write(SERVER,projectName,strlen(projectName));


	read(SERVER, &buffer, sizeof(int));
	char* manifestText = malloc(buffer*sizeof(char));

	read(SERVER, manifestText, buffer);

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
		liveHash = (char*)genLiveHash(clientManifest[i]->name);
		if(strcmp(liveHash, clientManifest[i]->hash) != 0){
			clientManifest[i]->verNum++;
			free(liveHash);
		}
	}
	
	//Pointer should be to server's .manifest text
	manEntry** serverManifest = readManifest("a2.manifest");
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
	
	send_file(commitName);
	
}

void pushProject(char * projectName){
	char* commitName = parseCommitName(projectName);
	if(access(commitName, F_OK) != 0){
		printf("There is no commit file, commit first\n");
		exit(0);
	}
	
	write(SERVER,"push", 5);
	
	int buffer = strlen(projectName);
	write(SERVER, &buffer,sizeof(int));
	write(SERVER,projectName,strlen(projectName));
	
	manEntry** commitArray = readCommit(commitName);
	int cEntries = MANIFEST_ENTRIES;
	
	char* tarName = mainCompress(projectName, commitArray, cEntries);
	
	send_file(tarName);
	
	remove(commitName);
	remove(tarName);
	free(tarName);
}

int send_file(char * path){
	int send_file = open (path, O_RDONLY);
	if (send_file<0){
		return 0;
	}
	int length = lseek(send_file,0,SEEK_END);
	lseek(send_file,0, SEEK_SET);
	write(SERVER, &length ,sizeof(int));
	char * buffer = malloc(length*sizeof(char));
	int bytes_read = 0;
	while (bytes_read<length){
		bytes_read += read(send_file,buffer,1024);
	}

	int bytes_written=0;
	while(bytes_written<length){
		bytes_written+=write(SERVER,buffer,strlen(buffer));

	}
}
