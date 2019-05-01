#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>


// char *  get_file_info(FILE *fd){
//     int file = open(fd,O_RDONLY, 0644);
// 	int filesize = lseek(file,0,SEEK_END);
//     read(fd,&filesize,4);
//     char * filename = malloc((filesize)*sizeof(char));
//     return filename;
// }


void the_input_command(char * buffer, void *vargp){
    int* fd = (int*)vargp;

    if (strcmp(buffer,"create")==0){
        

    }
    else if (strcmp(buffer,"checkout")==0){

    }
    else if (strcmp(buffer,"update")==0){

    }
    else if (strcmp(buffer,"upgrade")==0){

    }
    else if (strcmp(buffer,"commit")==0){

    }
    else if (strcmp(buffer,"push")==0){
        int buffer;
        read(*fd,&buffer,sizeof(int));
        char * file_name = malloc(buffer*sizeof(char)+1);
        read(*fd,file_name,buffer);
        read(*fd,&buffer,sizeof(int));
        char * actual_file = malloc(buffer*sizeof(char)+1);
        read(*fd, actual_file, buffer);
        
        //open a file with the name gotten and then write to it all the things that were in teh file 
        int new_file = open(file_name, O_CREAT | O_RDWR, 0644);
        write(new_file,actual_file,buffer);
        close(new_file);

        
    }
    else if (strcmp(buffer,"currenversion")==0){

    }
    else if (strcmp(buffer,"history")==0){

    }
    else if (strcmp(buffer,"rollback")==0){

    }




}


void error(const char *msg)
{
    perror(msg);
    exit(0);
}


void * get_buffer( FILE * fp){
    int file = open(".configure" ,O_RDONLY, 0644);
	int length = lseek(file,0,SEEK_END);
	// lseek(file,0, SEEK_SET);
}

void* func(void *vargp, FILE *file)
{
    int* fd = (int*)vargp;
    char command_buffer[256];

    printf("new connection\n");
    // this is the loop that is infinite and it always listening to a client reception 
    while (1)
    {
       // void * file_buffer= get_buffer(file);
       // double check with the vqlue of 0
        if (read(*fd, command_buffer, 255) >= 0)
        {   
            if (strcmp(command_buffer,"add")==0){
                printf("got here\n");
            }
            
        }
        return 0;
        //if (strcmp(buffer, "done") == 0)
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
