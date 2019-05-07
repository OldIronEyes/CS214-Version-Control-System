#include "WTF.h"

char* mainCompress(char* projectName, manEntry** entries, int cEntries){
	int comLen = 17 + strlen(projectName);
	
	int tarLen = 11 + strlen(projectName);
	
	int nLen = floor(log10(entries[0]->verNum)) + 1;
	char version[nLen+1];
	memset(version, 0x0, nLen+1);
	sprintf(version, "%d", entries[0]->verNum);
	comLen += nLen;
	tarLen += nLen;
	
	int i;
	for(i = 0; i < cEntries; i++){
		comLen += strlen(entries[i]->name) + 1;
	}
	
	char* command = malloc(comLen*sizeof(char));
	memset(command, 0x0, comLen);
	
	char* tarName = malloc(tarLen*(sizeof(char)));
	memset(tarName, 0x0, comLen);
	
	strncpy(command, "tar -zcf ", 9);
	
	strncat(command, projectName, strlen(projectName));
	strncpy(tarName, projectName, strlen(projectName));
	
	strncat(command, ".", 1);
	strncat(command, version, nLen);
	strncat(tarName, ".", 1);
	strncat(tarName, version, nLen);
	
	strncat(command, ".tar.gz ", 8);
	strncat(tarName, ".tar.gz ", 8);
	
	for(i = 0; i < cEntries; i++){
		strncat(command, entries[i]->name, strlen(entries[i]->name));
		strncat(command, " ", 1);
	}
	system(command);
	free(command);
	
	return tarName;
}

//Compresses all files in a server's manifest
char* serverCompress(char* projectName){
	char* manifestPath = malloc(((strlen(projectName)*2)+11)*sizeof(char));
	memset(manifestPath, 0x0, ((strlen(projectName)*2)+11));
	sprintf(manifestPath, "%s/%s.manifest", projectName, projectName);
	
	manEntry** manifest = readManifest(manifestPath);
	int cEntries = MANIFEST_ENTRIES;
	int i;
	int pathLen;
	
	for(i = 1; i < cEntries; i++){
		pathLen = strlen(manifest[i]->name) + strlen(projectName) + 2;
		char* filePath = malloc(pathLen);
		memset(filePath, 0x0, pathLen);
		sprintf(filePath, "%s/%s", projectName, manifest[i]->name);
		
		free(manifest[i]->name);
		manifest[i]->name = filePath;
	}
	
	char* tarName = mainCompress(projectName, manifest, cEntries);
	
	//Move the tar file
	int comLen = (strlen(tarName)+strlen(projectName)+6);
	char* command = malloc(comLen*sizeof(char));
	memset(command, 0x0, comLen);
	sprintf(command, "mv %s %s/", tarName, projectName);
	
	system(command);
	free(command);
	
	pathLen = strlen(tarName)+strlen(projectName)+2;
	char* tarPath = malloc(pathLen);
	sprintf(tarPath, "%s/%s", tarName, projectName);
	free(tarName);
	
	return tarPath;
}

//Extracts the files into the cwd
void mainExtract(char* tarName){
	int comLen = strlen(tarName) + 10;
	char* command = malloc(comLen*sizeof(char));
	sprintf(command, "tar -zxf %s", tarName);
	printf("%s\n", command);
	system(command);
	free(command);
	remove(tarName);
}

//Extracts a tar file into its project folder
void serverExtract(char* projectName, char* tarPath){
	int comLen = strlen(projectName) + strlen(tarPath) + 15;
	char* command = malloc(comLen*sizeof(char));
	sprintf(command, "tar -zxf %s -C %s/", tarPath, projectName);
	system(command);
	free(command);
}
