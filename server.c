#include "WTF.h"

void serverCreate(int fd){
    int buffer;
    read(fd,&buffer,sizeof(int));
    
    char * project_name = malloc(buffer * sizeof(char)+1);
    read(fd, project_name, buffer);
    
    if(mkdir(project_name, mode) != 0){
        printf("Unable to create the project\n");
    }
    return;
}

void serverDestroy(int fd){
    int input_buffer;
    read(fd,&input_buffer,sizeof(int));
    char* project_name = malloc(input_buffer*sizeof(char)+1);
    read(fd,project_name,input_buffer);
    if(check_if_there(project_name)==0){
        DIR* project = opendir(project_name);
        struct dirent* file;
        
        readdir(project);
        readdir(project);
        
        char* folderName = malloc(strlen(project_name)+2);
		strncpy(folderName, project_name, strlen(project_name));
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
		rmdir(project_name);
    }
    return;
}

void serverPush(int fd){
    int input_buffer;

    //Project Name
    read(fd, &input_buffer, sizeof(int));
    char * project_folder = malloc(input_buffer*sizeof(char)+1);
    read(fd,project_folder,input_buffer);

    //Filename
    read(fd,&input_buffer,sizeof(int));
    char * file_name = malloc(input_buffer*sizeof(char)+1);
    read(fd,file_name,input_buffer);
    
    //File
    read(fd,&input_buffer,sizeof(int));
    char * actual_file = malloc(input_buffer*sizeof(char)+1);
    read(fd, actual_file, input_buffer);


    char path [strlen(file_name)+strlen(project_folder)+2];
    memset(path,0x0,strlen(file_name)+strlen(project_folder)+2);
    sprintf(path,"%s/%s",project_folder, file_name);
    int new_file = open(path,O_CREAT | O_RDWR, 0644);
    if (new_file<0){
        return;
    }
    write(new_file,actual_file,strlen(actual_file));
    close(new_file);
    return;
}

void serverCommit(int fd){
    int buffer;

    //Size of project name
    read (fd, &buffer, sizeof(int));
    char * project_name = malloc(buffer*sizeof(char)+1);

    //project name
    read (fd, project_name,buffer);
    char* commit_name = malloc(strlen(project_name)*2+9);
    sprintf(commit_name, "%s/%s.commit", project_name, project_name);
    
    if (check_if_there(commit_name)!=0){
        printf("The file is not on the server side\n");
        return;
    }

    //Get manifest path
    char* manifest_path = malloc(strlen(project_name)*2+11);
    sprintf(manifest_path, "%s/%s.manifest", project_name, project_name);


    int contents = open(manifest_path, readFlag);
    struct stat file;
    stat(manifest_path, &file);
    char* manifestText = malloc(file.st_size*sizeof(char)+1);
    read(manifestText, contents, file.st_size);

    write(fd, file.st_size, sizeof(int));

    write(fd, manifestText, file.st_size);



    read(fd,&buffer,sizeof(int));
    char * actual_file =  malloc(buffer*sizeof(char));
    read(fd, actual_file, buffer);


    char* commit_path = malloc(strlen(project_name)*2+9);
    sprintf(commit_path, "%s/%s.commit", project_name, project_name);

    int commitName = open(commit_path,newFlag);
    write(commitName,actual_file, buffer);
    close(commitName);
    return;
}

void serverUpgrade(int fd){
    int buffer;
    read(fd,&buffer,sizeof(int));
    char * project_name = malloc(buffer * sizeof(char)+1);
    read(fd, project_name, buffer);
    if (check_if_there(project_name)!=0){
        printf("the file is not in the system\n");
        return;
    }
    
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
    write(fd, strlen(tar_name),sizeof(int));
    write(fd,tar_name,strlen(tar_name)+1);

    struct stat second;
    stat(tar_path,&second);
    // sending the tar file 
    write(fd, strlen(tar_path),sizeof(int));
    write(fd,tar_path,second.st_size);
}

int check_if_there(char * projectName){
	  DIR * project = opendir(projectName);
	if (ENOENT == errno){
		printf("The folder does not exist\n");
		return -1;
	}
	return 0;
}
