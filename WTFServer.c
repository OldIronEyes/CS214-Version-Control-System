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
#include <dir.h>

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

void check_arg(char * buffer, int fd){
    // this is skelton for the main argu
    // use a switch case much much better 
    
    if (strcmp(buffer,"push")==0){
        int input_buffer;
        read(fd,&input_buffer,sizeof(int));
        char * file_name = malloc(input_buffer*sizeof(char)+1);
        read(fd,file_name,input_buffer);
        read(fd,&buffer,sizeof(int));
        char * actual_file = malloc(input_buffer*sizeof(char)+1);
        read(fd, actual_file, input_buffer);
        char path [strlen(file_name)+2];
        sprintf(path,"./%s",file_name);
        int new_file = open(path,O_CREAT | O_RDWR, 0644);
        if (new_file<0){
            return ;
        }
        write(new_file,actual_file,strlen(actual_file));
        close(new_file);
    }
    else if (strcmp(buffer,"destroy")==0){
       int input_buffer;
       read(fd,&input_buffer,sizeof(int));
       char* project_name = malloc(input_buffer*sizeof(char)+1);
       read(fd,project_name,input_buffer);
       // needs to give it a path over here for it to check.
       int check_for_file = access(project_name/project_name.manifest, F_OK); 
       if (check_for_file=0){
           destroyProject(project_name);
       }else {
           printf("you are trying to destroy a project that does not exist\n");
           return;
       }
       
       destroyProject(project_name);
       return;
    }
    
    else if (strcmp(buffer,"create")==0){
        int buffer;
        read(fd,&buffer,sizeof(int));
        char * project_name = malloc(buffer * sizeof(char)+1);
        read(fd, project_name, buffer);
        char * max_path = malloc(4096*sizeof(char));
        getcwd(max_path,4096);
        max_path = realloc(max_path,4096+strlen(argv[2]+1));
        strcat(max_path,argv[2]);
        int project_folder = mkdir(max_path,0777);
        
         
    
    }

}

void* func(void *vargp)
{
    int* fd = (int*)vargp;
    char buffer[256];
    printf("new connection\n");
    while (1)
    {


        if (read(*fd, buffer, 255) < 0)
        {
            perror("read");
            close(*fd);
            return NULL;
        }
        //printf("%s\n going to the arguments yay\n",buffer);
        check_arg(buffer,*fd);
       
        
        printf("This is the end of the recieving\n");
        break;
        if (strcmp(buffer, "done") == 0)
            break;
    }
    close(*fd);
    return NULL;
}

int main(int argc, char *argv[])
{
	if (atoi(argv[1])<8000 || atoi(argv[1])>64000){
		printf("PORT must be between 8k adn 64k\n");
		return 0;
	}
    int sockfd, newsockfd, portno;
    socklen_t clilen_addr_size;
    struct sockaddr_in serv_addr, cli_addr;
    pthread_t thread;
    if (argc < 2) {
        fprintf(stderr,"ERROR, no port provided\n");
        exit(1);
    }
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");
    portno = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    int a = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &a , sizeof(int)) < 0){
        
    }
    if (bind(sockfd, (struct sockaddr *) &serv_addr,
             sizeof(serv_addr)) < 0)
        error("ERROR on binding");
    listen(sockfd,5);
    clilen_addr_size = sizeof(cli_addr);
    while (1)
    {
        newsockfd = accept(sockfd,
                           (struct sockaddr *) &cli_addr,
                           &clilen_addr_size);
        if (newsockfd < 0)
            error("ERROR on accept");
        pthread_create(thread, NULL, func, (void*)(&newsockfd));
        
    }
    
    close(sockfd);
    return 0;
}
