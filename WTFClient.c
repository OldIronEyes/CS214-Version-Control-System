#include "WTF.h"

/*
 * Client should attempt to connect every three 
 * seconds until successful or interrupted by SIGINT
 * returns 1 when a connection is established
 */

int connect(char* IPAddress, int portNumber){

}

/*
 * Should write/update the .config file to update the
 * connection variables as necessary, and update the global
 * variables defined in WTF.h
 */

void configure(char* newIP, int newPort){

}

void writeManifest(){

}

void checkout(char* projectName){

}

void update(char* projectName){

}

void upgrade(char* projectName){

}

void commit(char* projectName){

}

void push(char* projectName){

}

void create(char* projectName){

}

void destroy(char* projectName){

}

void add(char* projectName, char* fileName){

}

void remove(char* projectName, char* fileName){

}

void currentVersion(char* projectName){

}

void history(char* projectName){

}

void rollBack(char* projectName, int versionNumber){

}


/*
 * This code looks disgusting but there's no way to use strings in
 * switch statements in C, so this is the next best option. Each of the
 * printf statements should eventually be replaced with the appropriate
 * function call.
 */

int main(int argc, char *argv[]){
	if(argv[1] == "checkout"){
	
		printf("checkout\n");
	
	} else if (argv[1] == "update"){
	
		printf("update\n");
	
	} else if (argv[1] == "upgrade"){
	
		printf("upgrade\n");
	
	} else if (argv[1] == "commit"){
	
		printf("commit\n");
	
	} else if (argv[1] == "push"){
	
		printf("push\n");
	
	} else if (argv[1] == "create"){
	
		printf("create\n");
	
	} else if (argv[1] == "destroy"){
	
		printf("destroy\n");
	
	} else if (argv[1] == "add"){
	
		printf("add\n");
	
	} else if (argv[1] == "remove"){
	
		printf("remove\n");
	
	} else if (argv[1] == "currentversion"){
	
		printf("currentversion\n");
	
	} else if (argv[1] == "history"){
	
		printf("history\n");
	
	} else if (argv[1] == "rollback"){
	
		printf("rollback\n");
	
	} else {
	
		printf("Invalid Command\n");
	}
	
}
