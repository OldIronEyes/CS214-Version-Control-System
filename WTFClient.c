#include "WTF.h"

int main (int argc, char ** argv){
	if (argc < 3 ){
		printf("you must put in a valid argument\n");
		exit(0);
	}
	node * LL;
	int server;
 	//==============================FUNCTIONS THAT DO NOT NEED TO CONNECT TO THE SERVER ====================================
	if (strcmp(argv[1],"configure")==0){
		if (argc<4){
			printf("you need to put a valid host name and a port number to connect to\n");
			exit(0);
		}
		configure(argv[2],argv[3]);
		tokenize();
	}	
	else if(strcmp(argv[1],"add")){
		if (argc<4){
			printf("you must have a projectname and a filename\n");
			exit(0);
		}
		addFile(argv[2],argv[3]);
	}
	else if(strcmp(argv[1],"remove")==0){
		if (argc<4){
			printf("you must have a projectname and a filename\n");
			exit(0);
		}
		removeFile(argv[2],argv[3]);
	}
	//================================== FUNCTIONS THAT REQUIRE THE SERVER =====================================
	else if (strcmp(argv[1],"create")==0){
		int server = connect_server(IP,PORT);
		write(server,"create", 7);
		write(server,strlen(argv[2]+1),sizeof(int));
		write(server,argv[2],strlen(argv[2])+1);
		createProject(argv[2]);
		
	}
	else if (strcmp(argv[1],"push")==0){
		server = connect_server(IP,PORT);
		write(server,"push", 5);
		
		// need to change the thing over here and also compress the file here 
		send_file("WTFserver.h",server);
	}
	else if (strcmp(argv[1],"destroy")==0){
		int socket = connect_server(IP,PORT);
		write(server, "destroy", 8);
		int length;
		write(socket,&length,sizeof(int));
		write(socket,argv[2],strlen(argv[2])+1);
		//check for the read here 
		destroyProject(argv[2]);
	}
	else if (strcmp(argv[1],"update")==0){
		int server = connect_server(IP,PORT);
		write(server, "update",6);
		updateProject(argv[2]);
		
	}

	
	printf("%s\n",IP);
	printf("%d\n",PORT);

	return 0;
}
































// int main(int argc, char* argv[]){
// 	if(strcmp(argv[1], "configure") == 0){
// 		printf("configure");
// 	}

// 	else if (strcmp(argv[1],"create") = 0){
// 		if(argc != 2){
// 			printf("Incorrect number of arguments");
// 		} else {
// 			createProject(argv[2]);
// 		}
// 	}

// 	else if (strcmp(argv[1],"destroy") = 0){
// 		if(argc != 2){
// 			printf("Incorrect number of arguments");
// 		} else {
// 			createProject(argv[2]);
// 		}
// 	}

// 	else if (strcmp(argv[1],"add") = 0){
// 		if(argc != 3){
// 			printf("Incorrect number of arguments");
// 		} else {
// 			addFile(argv[2], argv[3]);
// 		}
// 	}

// 	else if (strcmp(argv[1],"remove") == 0){
// 		if(argc != 3){
// 			printf("Incorrect number of arguments");
// 		} else {
// 			removeFile(argv[2], argv[3]);
// 		}
// 	}

// 	else if (strcmp(argv[1],"checkout") = 0){
// 		printf("checkout");
// 	}

// 	else if (strcmp(argv[1],"update") = 0){
// 		printf("update");
// 	}

// 	else if (strcmp(argv[1],"upgrade") = 0){
// 		printf("upgrade");
// 	}

// 	else if (strcmp(argv[1],"commit") = 0){
// 		printf("commit");
// 	}

// 	else if (strcmp(argv[1],"push") = 0){
// 		printf("push");
// 	}

// 	else if (strcmp(argv[1],"currentversion") = 0){
// 		printf("current version");
// 	}

// 	else if (strcmp(argv[1],"history") = 0){
// 		printf("history");
// 	}

// 	else if (strcmp(argv[1],"rollback") = 0){
// 		printf("rollback");
// 	}

// 	else {
// 		printf("Invalid Comand");
// 	}
// }
