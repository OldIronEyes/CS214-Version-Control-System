#include "WTFClient.h"

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
