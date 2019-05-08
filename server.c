#include "WTF.h"

void serverCreate(int fd){
	printf("creating project\n");
    int buffer = 0;
    
    //Project Name
    while(read(fd, &buffer, sizeof(int)) < sizeof(int));
    
    char* projectName = malloc(buffer*sizeof(char)+1);
    
	while(read(fd, projectName, buffer) < buffer);
    
    printf("%s\n", projectName);
    if(mkdir(projectName, S_IRUSR | S_IWUSR | S_IXUSR) != 0){
        printf("Unable to create the project, %d\n", errno);
    }
    return;
}

void serverDestroy(int fd){
    int buffer;
    
    while(read(fd,&buffer,sizeof(int)) < sizeof(int));

    char* projectName = malloc(buffer*sizeof(char)+1);
    while(read(fd, projectName, buffer) < buffer);

    if(check_if_there(projectName)==0){
        DIR* project = opendir(projectName);
        struct dirent* file;
        
        readdir(project);
        readdir(project);
        
        char* folderName = malloc(strlen(projectName)+2);
		strncpy(folderName, projectName, strlen(projectName));
		strncat(folderName, "/", 1);
		
		while(file = readdir(project)){
			char* filePath = malloc(strlen(folderName)+strlen(file->d_name)+1);
			strncpy(filePath, folderName, strlen(folderName));
			strncat(filePath, file->d_name, strlen(file->d_name));
			remove(filePath);
			free(filePath);
			printf("%s\n", file->d_name);
		}
		free(folderName);
		rmdir(projectName);
    }
    return;
}

void serverPush(int fd){
    int buffer;

    //Project Name
    while(read(fd, &buffer, sizeof(int)) < sizeof(int));
    char* projectName = malloc(buffer*sizeof(char)+1);
    while(read(fd,projectName,buffer) < buffer);


    //Filename
    while(read(fd,&buffer,sizeof(int)) < sizeof(int));
    char* fileName = malloc(buffer*sizeof(char)+1);
    while(read(fd,fileName,buffer) < buffer);

    
    //File
    while(read(fd,&buffer,sizeof(int)) < sizeof(int));
    char* fileContents = malloc(buffer*sizeof(char)+1);
    while(read(fd, fileContents, buffer) < buffer);


    char filePath[strlen(fileName) + strlen(projectName)+2];
    memset(filePath,0x0,strlen(fileName)+strlen(projectName)+2);
    
    sprintf(filePath,"%s/%s",projectName, fileName);
    int contents = open(filePath, newFlag, mode);
    if (contents < 0){
        return;
    }
    write(contents, fileContents, strlen(fileContents));
    close(contents);
    return;
}

void serverCommit(int fd){
    int buffer;

    //Project Name from Client
    while(read (fd, &buffer, sizeof(int)) < sizeof(int));
    char* projectName = malloc(buffer*sizeof(char)+1);
    while(read (fd, projectName, buffer) < buffer);

    if (check_if_there(projectName) != 0){
        printf("the file is not in the system\n");
      return;
    }
    
    //Get Commit Path
    char* commitPath = malloc(strlen(projectName)*2+9);
    sprintf(commitPath, "%s/%s.commit", projectName, projectName);
    remove(commitPath);

    //Get Manifest path
    char* manifestPath = malloc(strlen(projectName)*2+11);
    sprintf(manifestPath, "%s/%s.manifest", projectName, projectName);

	//Read Manifest into memory
    int contents = open(manifestPath, readFlag);
    struct stat file;
    stat(manifestPath, &file);
    char* manifestText = malloc(file.st_size*sizeof(char)+1);
    read(contents, manifestText, file.st_size);
    close(contents);

	//Write Manifest to Client
	buffer = file.st_size;
    while(write(fd, &buffer, sizeof(int)) < sizeof(int));
    while(write(fd, manifestText, file.st_size) < sizeof(int));

    free(manifestText);
    
    //Get New Commit from Clietnt
    while(read(fd, &buffer, sizeof(int)) < sizeof(int));
    char* commitText =  malloc(buffer*sizeof(char));
    while(read(fd, commitText, buffer) < buffer);


	//Write Commit to file
    int commitName = open(commitPath, newFlag, mode);
    write(commitName, commitText, buffer);
    close(commitName);
    return;
}

void serverUpgrade(int fd){
    int buffer;
    
    //Get Project Name
    while(read(fd, &buffer, sizeof(int)) < sizeof(int));
    char* projectName = malloc(buffer * sizeof(char)+1);
    while(read(fd, projectName, buffer) < buffer);

    
    if (check_if_there(projectName)!=0){
        printf("the file is not in the system\n");
        return;
    }
    
    char* tarPath = serverCompress(projectName);
    
    //Get Tar Name
    char * tarName = malloc (strlen(tarPath)*sizeof(char)+1);
    memset(tarName,0x0,strlen(tarPath)+1);
    int leader, trailer;
    leader = strlen(tarPath);
    trailer = leader;
    while(tarPath[trailer] != '/'){
        trailer--;
    }
    strncpy(tarName,(char*)tarPath+trailer, (leader-trailer));
    
    //Read Tar File into Memory
    struct stat tarStat;
    stat(tarPath,&tarStat);
    int contents = open(tarPath, readFlag);
    char* tarText = malloc(tarStat.st_size*sizeof(char)+1);
    read(contents, tarText, tarStat.st_size);
    close(contents);
    
    //Send Tar Name
    buffer = strlen(tarName)+1;
    while(write(fd, &buffer,sizeof(int)) < sizeof(int));
    while(write(fd, tarName, strlen(tarName)) < strlen(tarName));

	
    //Send Tar File 
    buffer = tarStat.st_size;
    while(write(fd, &buffer,sizeof(int)) < sizeof(int));
    while(write(fd,tarText,tarStat.st_size) < tarStat.st_size);

    remove(tarPath);
    free(projectName);
    free(tarPath);
    free(tarName);
    free(tarText);
}

int check_if_there(char * projectName){
	  DIR * project = opendir(projectName);
	if (ENOENT == errno){
		return -1;
	}
	return 0;
}
