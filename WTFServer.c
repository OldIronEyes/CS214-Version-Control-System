#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <ctype.h>
#include "WTF.h"
#include <dirent.h>

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

void check_arg(char * buffer, int fd){
    if (strcmp(buffer,"create") == 0){
        serverCreate(fd);
        
    }
    else if (strcmp(buffer,"destroy") == 0){
        serverDestroy(fd);
        
    }
    else if (strcmp(buffer,"checkout") == 0){
        serverDestroy(fd);
        
    }
    else if (strcmp(buffer,"update")==0){
        serverDestroy(fd);
        
    }
    else if (strcmp(buffer,"upgrade")==0){
        serverDestroy(fd);
        
    }
    else if (strcmp(buffer,"commit") == 0){
        serverCommit(fd);
    }
    if (strcmp(buffer,"push") == 0){
        serverPush(fd);
    }
}

void* func(void *vargp){
	printf("New Thread\n");
    int* fd = (int*)vargp;
    char buffer[256];
    while (1)
    {
        if (read(*fd, buffer, 255) < 0)
        {
            perror("read");
            close(*fd);
            return NULL;
        }
        check_arg(buffer,*fd);
       
        break;
    }
    close(*fd);
    return NULL;
}

int main(int argc, char *argv[])
{
	if (atoi(argv[1])<8000 || atoi(argv[1])>64000){
		printf("PORT must be between 8,000 and 64,000\n");
		return 0;
	}
    int sockfd, newsockfd, portno;
    socklen_t clilen_addr_size;
    struct sockaddr_in serv_addr, cli_addr;
    pthread_t thread;
    if (argc < 2) {
        fprintf(stderr,"ERROR, no port provided\n");
        exit(0);
    }
    
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0){
		error("ERROR opening socket\n");
		exit(0);
    }

    portno = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    int a = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &a , sizeof(int)) < 0){
        
    }
    if (bind(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0){
    	error("ERROR on binding");
    	exit(0);
    }
    listen(sockfd,5);
    printf("Listening\n");
    clilen_addr_size = sizeof(cli_addr);
    while (1)
    {
		newsockfd = accept(sockfd,(struct sockaddr *) &cli_addr,&clilen_addr_size);
        if (newsockfd < 0){
            error("ERROR on accept");
        } else {
        	printf("Connected\n");
        	pthread_create(&thread, NULL, func, (void*)(&newsockfd));
        }
        
    }
    
    close(sockfd);
    return 0;
}
