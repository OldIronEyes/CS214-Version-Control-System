#include "WTF.h"
#include "Connection_from_client.c"

int main(int argc, char* argv[]){

	if (argc<3){
		printf("please input a proper amount of arguments\n");
		return 0;
	}
	int command_buffer= strlen(argv[1])+1;


//	CONFIGURE %100 WORKS----------------- DO NOT TOUCH-------------------------
	if(strcmp(argv[1], "configure") == 0){
		if (argc<4){
			printf("you need to input an IP address and a port\n");
			exit(0);
		}
		configure(argv[2],argv[3]);
		tokenize();
	}
	int server= connect_server(IP,PORT);
	write(server,argv[1],command_buffer);

	// else if (strcmp(argv[1],"create") == 0){
	// 	if(argc != 2){
	// 		printf("Incorrect number of arguments");
	// 	} else {
	// 		createProject(argv[2]);
	// 	}
	// }

	// else if (strcmp(argv[1],"destroy") == 0){
	// 	if(argc != 2){
	// 		printf("Incorrect number of arguments");
	// 	} else {
	// 		createProject(argv[2]);
	// 	}
	// }

	// else if (strcmp(argv[1],"add") == 0){
	// 	if(argc != 3){
	// 		printf("Incorrect number of arguments");
	// 	} else {
	// 		addFile(argv[2], argv[3]);
	// 	}
	// }

	// else if (strcmp(argv[1],"remove") == 0){
	// 	if(argc != 3){
	// 		printf("Incorrect number of arguments");
	// 	} else {
	// 		removeFile(argv[2], argv[3]);
	// 	}
	// }

	// else if (strcmp(argv[1],"checkout") == 0){
	// 	printf("checkout");
	// }

	// else if (strcmp(argv[1],"update") == 0){
	// 	printf("update");
	// }

	// else if (strcmp(argv[1],"upgrade") == 0){
	// 	printf("upgrade");
	// }

	// else if (strcmp(argv[1],"commit") == 0){
	// 	printf("commit");
	// }

	// else if (strcmp(argv[1],"push") == 0){
	// 	printf("push");
	// }

	// else if (strcmp(argv[1],"currentversion") == 0){
	// 	printf("current version");
	// }

	// else if (strcmp(argv[1],"history") == 0){
	// 	printf("history");
	// }

	// else if (strcmp(argv[1],"rollback") == 0){
	// 	printf("rollback");
	// }

	// else {
	// 	printf("Invalid Command");
	// }
}
