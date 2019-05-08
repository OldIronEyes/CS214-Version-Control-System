#include "WTF.h"

int main(int argc, char** argv){
	if(argc == 2){
		system("server/WTFServer 9000");
		exit(0);
	}
	
	int i;
	
	
	printf("1. Attempting to Create a Project without Configuring the Client\n");
	i = fork();
	if(!i){
		system("client/WTFClient create test");
		exit(0);
	}
	wait();
	
	printf("2. Calling Configure to write a .configure file\n");
	i = fork();
	if(!i){
		system("client/WTFClient configure 127.0.0.1 9000");
		exit(0);
	}
	wait();
	
	printf("3. Creating a new Project\n");
	i = fork();
	if(!i){
		system("client/WTFClient create test");
		exit(0);
	}
	wait();
	
	printf("4. Attempting to create a Project with the same name as one that already exists\n");
	i = fork();
	if(!i){
		system("client/WTFClient create test");
		exit(0);
	}
	wait();
	
	printf("5. Adding three files to the Project\n");
	i = fork();
	if(!i){
		system("touch client/a.txt");
		system("touch client/b.txt");
		system("touch client/c.txt");
		system("client/WTFClient add test a.txt");
		system("client/WTFClient add test b.txt");
		system("client/WTFClient add test c.txt");
		exit(0);
	}
	wait();
	
	printf("6. Attempting to add a file that is already in the Project\n");
	i = fork();
	if(!i){
		system("client/WTFClient add test a.txt");
		exit(0);
	}
	wait();
	
	printf("7. Attempting to Push without a .commit file\n");
	i = fork();
	if(!i){
		system("client/WTFClient push test");
		exit(0);
	}
	wait();
	
	printf("8. Commiting the Project\n");
	i = fork();
	if(!i){
		system("client/WTFClient commit test");
		exit(0);
	}
	wait();
	
	printf("9. Pushing a Commit with new Project files\n");
	i = fork();
	if(!i){
		system("client/WTFClient push test");
		exit(0);
	}
	wait();
	
	printf("10. Attempting to Commit without any file differences\n");
	i = fork();
	if(!i){
		system("client/WTFClient commit test");
		exit(0);
	}
	wait();
	
	printf("11. Modifying a file, then commiting\n");
	i = fork();
	if(!i){
		int contents = open("client/a.txt", O_RDWR | O_APPEND, S_IRUSR | S_IWUSR | S_IXUSR);
		write(contents, "stuff", 5);
		close(contents);
		system("client/WTFClient commit test");
		exit(0);
	}
	wait();
	
	printf("12. Pushing a Commit with a modified Project file\n");
	i = fork();
	if(!i){
		system("client/WTFClient push test");
		exit(0);
	}
	wait();
	
	printf("13. Attempting to Upgrade the Project without a .update file\n");
	i = fork();
	if(!i){
		system("client/WTFClient upgrade test");
		exit(0);
	}
	wait();
	
	printf("14. Attempting to Update the Project without any removed Project files\n");
	i = fork();
	if(!i){
		system("client/WTFClient update test");
		exit(0);
	}
	wait();
	
	printf("15. Removing a Project file\n");
	i = fork();
	if(!i){
		system("client/WTFClient remove test a.txt");
		exit(0);
	}
	wait();
	
	printf("16. Attempting to Remove a file that is not being tracked\n");
	i = fork();
	if(!i){
		system("client/WTFClient remove test a.txt");
		exit(0);
	}
	wait();
	
	printf("17. Removing Project files, then Update the Project\n");
	i = fork();
	if(!i){
		system("client/WTFClient update test a.txt");
		exit(0);
	}
	wait();
	
	printf("18. Upgrading the Project\n");
	i = fork();
	if(!i){
		system("client/WTFClient upgrade test");
		exit(0);
	}
	wait();
	
	printf("19. Modifing a Project file and attempting to Update\n");
	i = fork();
	if(!i){
		int contents = open("client/b.txt", O_RDWR | O_APPEND, S_IRUSR | S_IWUSR | S_IXUSR);
		write(contents, "stuff", 5);
		close(contents);
		system("client/WTFClient update test");
		exit(0);
	}
	wait();
	
	printf("20. Destroying the Project\n");
	i = fork();
	if(!i){
		system("client/WTFClient destroy test");
		exit(0);
	}
	wait();
	
	printf("21. Attempting to Destroy the Project when it doesn't exist\n");
	i = fork();
	if(!i){
		system("client/WTFClient destroy test");
		exit(0);
	}
	wait();
}
