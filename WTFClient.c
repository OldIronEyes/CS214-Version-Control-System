#include "WTF.h"

int main (int argc, char ** argv){
	if (argc < 3 ){
		printf("you must put in a valid argument\n");
		exit(0);
	}

	if (strcmp(argv[1],"configure") == 0){
		if (argc<4){
			printf("you need to put a valid host name and a port number to connect to\n");
			exit(0);
		}
		configure(argv[2],argv[3]);
		exit(0);
	}	
	else if(strcmp(argv[1],"add") == 0){
		if (argc<4){
			printf("you must have a projectname and a filename\n");
			exit(0);
		}
		addFile(argv[2],argv[3]);
		exit(0);
	}
	else if(strcmp(argv[1],"remove") == 0){
		if (argc<4){
			printf("you must have a projectname and a filename\n");
			exit(0);
		}
		removeFile(argv[2],argv[3]);
		exit(0);
	}
	
	readConfigure();
	connectServer(IP,PORT);
	
	if (strcmp(argv[1],"create") == 0){
		createProject(argv[2]);
		exit(0);
	}
	else if (strcmp(argv[1],"destroy") == 0){
		destroyProject(argv[2]);
		exit(0);
	}
	else if (strcmp(argv[1],"update") == 0){
		updateProject(argv[2]);
		exit(0);
	}
	else if (strcmp(argv[1],"upgrade") == 0){
		upgradeProject(argv[2]);
		exit(0);
	}
	else if (strcmp(argv[1],"commit") == 0){
		commitProject(argv[2]);
		exit(0);
	}
	else if (strcmp(argv[1],"push") == 0){
		pushProject(argv[2]);
		exit(0);
	}
	else if (strcmp(argv[1],"currentversion") == 0){
		printf("current version");
		exit(0);
	}
}

