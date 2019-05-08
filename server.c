#include "WTF.h"

void serverCreate(int fd){
	//Recieve 2
    int input_buffer;
    read(fd,&input_buffer,sizeof(int));
    
    char* projectName = malloc(input_buffer*sizeof(char)+1);
    read(fd,projectName,input_buffer);
    
    if(mkdir(projectName, S_IRUSR | S_IWUSR | S_IXUSR) != 0){
        printf("Unable to create the project\n");
    }
    char* manifestName = malloc(strlen(projectName)*2+10);
    sprintf(manifestName, "%s/%s.manifest", projectName, projectName);
	int contents = open(manifestName, newFlag, mode);
	manEntry* newProject = newManEntry(manifestName);
	writeManEntry(newProject, contents);
	close(contents);
	free(manifestName);
	freeManEntry(newProject);
}

void serverDestroy(int fd){
	//Read project Name
    int input_buffer;
    read(fd,&input_buffer,sizeof(int));
    char* project_name = malloc(input_buffer*sizeof(char));
    read(fd,project_name,input_buffer);
    printf("%s\n", project_name);

    DIR* project = opendir(project_name);
    struct dirent* file;
        
    readdir(project);
    readdir(project);
        
    char* folderName = malloc(strlen(project_name)+2);
    memset(folderName,0x0,strlen(project_name)+2);
	strncpy(folderName, project_name, strlen(project_name));
	strncat(folderName, "/", 1);
		
	while(file = readdir(project)){
		char* filePath = malloc(strlen(folderName)+strlen(file->d_name)+1);
		strncpy(filePath, folderName, strlen(folderName));
		strncat(filePath, file->d_name, strlen(file->d_name));
		printf("%s\n", filePath);
		remove(filePath);
		free(filePath);
	}
	free(folderName);
	rmdir(project_name);
    return;
}

void serverupdate(int fd){
	//read project name
	int buffer;
	read(fd, &buffer, sizeof(int));
	char * project_name = malloc(buffer * sizeof(char));
	read(fd, project_name, buffer);
	
	//parse manifest path
	char* manifestPath = malloc(strlen(project_name)*2+10);
	memset(manifestPath, 0x0, strlen(project_name)*2+10);
	sprintf(manifestPath, "%s/%s.manifest", project_name, project_name);
	
	//read manifest
	struct stat manifest;
	stat(manifestPath, &manifest);
	int contents = open(manifestPath, readFlag);
	char* manifestText = malloc(manifest.st_size+1);
	memset(manifestText, 0x0, manifest.st_size+1);
	read(contents, manifestText, manifest.st_size);
	close(contents);
	
	//send manifest
	buffer = manifest.st_size;
	write(SERVER, &buffer, sizeof(int));
	write(SERVER, manifestText, buffer);
}

void serverUpgrade(int fd){
	//get project name
    int buffer;
    read(fd,&buffer,sizeof(int));
    char * project_name = malloc(buffer * sizeof(char));
    read(fd, project_name, buffer);
    
    char * tar_path = serverCompress(project_name);
    
    //Get tar name
    char * tar_name = malloc (strlen(tar_path)*sizeof(char));
    memset(tar_name,0x0,strlen(tar_path));
    int leader, trailer;

    leader = strlen(tar_path);
    trailer = leader;

    while(tar_path[trailer] != '/'){
        trailer--;
    }

    strncpy(tar_name,(char*)tar_path+trailer, leader-trailer);
    // sending the file name 
    
    buffer = strlen(tar_name);
    write(fd, &buffer,sizeof(int));
    write(fd,tar_name,strlen(tar_name));

    struct stat second;
    stat(tar_path,&second);
    // sending the tar file 
    buffer = strlen(tar_path) +1;
    write(fd, &buffer,sizeof(int));
    write(fd,tar_path,second.st_size);
}

void serverCommit(int fd){
    int buffer;

    //Read 2
    read (fd, &buffer, sizeof(int));
    char * project_name = malloc(buffer*sizeof(char)+1);
    read (fd, project_name,buffer);

    char* commit_path = malloc(strlen(project_name)*2+9);
    sprintf(commit_path, "%s/%s.commit", project_name, project_name);
    
    if (check_if_there(commit_path)!=0){
        printf("There is no .commit file is not on the server side\n");
        exit(0);
    }

    //Get manifest path
    char* manifest_path = malloc(strlen(project_name)*2+11);
    sprintf(manifest_path, "%s/%s.manifest", project_name, project_name);

	//Read manifest
    int contents = open(manifest_path, readFlag);
    struct stat file;
    stat(manifest_path, &file);
    char* manifestText = malloc(file.st_size*sizeof(char)+1);
    read(contents, manifestText, file.st_size);
    close(contents);
	
	//Write manifest
	buffer = file.st_size;
    write(fd, &buffer, sizeof(int));
    write(fd, manifestText, file.st_size);

	//Read 3
    read(fd,&buffer,sizeof(int));
    char * actual_file =  malloc(buffer*sizeof(char));
    read(fd, actual_file, buffer);

    int commitName = open(commit_path,newFlag,mode);
    write(commitName,actual_file, buffer);
    close(commitName);
    return;
}

void serverPush(int fd){
    int buffer;

    //Read 2
    read(fd, &buffer, sizeof(int));
    char * projectName = malloc(buffer*sizeof(char)+1);
    read(fd,projectName,buffer);

    //Filename
    read(fd,&buffer,sizeof(int));
    char * file_name = malloc(buffer*sizeof(char)+1);
    read(fd,file_name,buffer);
    
    //File
    read(fd,&buffer,sizeof(int));
    char * actual_file = malloc(buffer*sizeof(char)+1);
    read(fd, actual_file, buffer);


    char path [strlen(file_name)+strlen(projectName)+2];
    memset(path,0x0,strlen(file_name)+strlen(projectName)+2);
    sprintf(path,"%s/%s",projectName, file_name);
    int new_file = open(path,O_CREAT | O_RDWR, 0644);
    if (new_file<0){
        return;
    }
    write(new_file,actual_file,strlen(actual_file));
    close(new_file);
    serverExtract(projectName, path);
    return;
}

int check_if_there(char * projectName){
	  DIR * project = opendir(projectName);
	if (ENOENT == errno){
		printf("The folder does not exist\n");
		return -1;
	}
	return 0;
}
