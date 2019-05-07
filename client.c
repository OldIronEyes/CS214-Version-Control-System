#include "WTF.h"

int check_if_there(char * projectName){
	  DIR * project = opendir(projectName);
	if (ENOENT == errno){
		printf("The folder does not exist\n");
		return -1;
	}   
	return 0;    
}

void createProject(char* projectName){
	//TODO check if the project already exists on the server
	int server = connect_server(IP,PORT);
	write(server,"create", 7);

	write(server,strlen(projectName)+1,sizeof(int));
	write(server,projectName,strlen(projectName)+1);

	
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
	int server = connect_server(IP,PORT);
	write(server, "destroy", 8);

	int length;
	write(socket,&length,sizeof(int));
	write(socket,projectName,strlen(projectName)+1);

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

	int server = connect_server(IP,PORT);
	write(server, "update",6);

	int length;
	read(server, &length, sizeof(int));
	char* serverManifestText = malloc((length+1)*sizeof(char));
	memset(serverManifestText, 0x0, length+1);
	read(server,serverManifestText,length);
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
	
	//TODO compareUpdateManifests(clientManifest, cEntries, serverManifest, sEntries);
	
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


int send_file(char * path, int socket){
	int send_file = open (path, O_RDONLY);
	if (send_file<0){
		return 0;
	}
	int length = lseek(send_file,0,SEEK_END);
	lseek(send_file,0, SEEK_SET);
	write(socket, &length ,sizeof(int));
	char * buffer = malloc(length*sizeof(char));
	int bytes_read = 0;
	while (bytes_read<length){
		bytes_read += read(send_file,buffer,1024);
	}

	printf("this is the buffer:\n%s\n",buffer);
	int bytes_written=0;
	while(bytes_written<length){
		bytes_written+=write(socket,buffer,strlen(buffer));
		printf("%d\n",bytes_written);
	}
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

void pushFile(char * projectName){
	int server = connect_server(IP,PORT);
	write(server,"push", 5);
	
	// need to change the thing over here and also compress the file here 
	write(server, strlen(projectName),sizeof(int));
	write(server,projectName,strlen(projectName)+1);
	// need to compress file here so that we can send that file 
	send_file("WTFserver.h",server);
}
























