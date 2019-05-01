#include <netdb.h> 
#include <stdio.h> 
#include <stdlib.h> 
//#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h>
#include <netinet/in.h>
#include <ctype.h>
//#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#include "WTF.h"


void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int connect_server (char * Ip, char *  port){
	int sockfd, portno, n, status, message_size, sent_size, total_size;
    struct sockaddr_in serv_addr;
    struct hostent *server;

	char recv_buffer[1024], send_buffer[1024], *tmp;

	portno = atoi(port);
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) 
        error("ERROR opening socket");
    server = gethostbyname(Ip);
	if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
	bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");
	
	printf("the connection is good\n");

	return sockfd;
}


// -----------------------------THE WRITE IS THE THING THAT SENDS THE ENTIRE FILE---------------------- IF NEEDED ASK YITZY AGAIN BUT MAKES SENSE ---------------------------------------------------
//  	 while (1)
//     {
//         printf(" the connection has been established\n");
// 		break;
    
// 		while (total_size < message_size)
//         {
//             if ((sent_size = write(sockfd, tmp, message_size - total_size)) < 0)
//             
//                 error("ERROR writing to socket");
//                 break;
//             }
//             total_size += sent_size;
//             tmp += sent_size;
//         }
//         if (total_size == message_size)
//             printf("message sent\n");
//         if (strcmp(send_buffer, "done") == 0)
//             break;
		
//     }
//     close(sockfd);


//  }

void configure ( char* IPAddress, char* port ){

	// writes into the file 
	int removed = remove( ".configure");
	int port_Number = strtol(port, NULL, 10);
	if( port_Number<8000 || port_Number>63999){
		fprintf(stderr, "This is not a valid port number please enter another. Must be greater than 8K and less than 64k\nEnded\n");
		exit(0);
	}
	int file_desciptor;
	file_desciptor = open(".configure", O_CREAT | O_RDWR, 0644);
	write(file_desciptor,IPAddress, strlen(IPAddress));
	write(file_desciptor, " ", 1);
	write(file_desciptor, port,strlen(port));
	close(file_desciptor);

}

node * tokenize(){

	// tokenizes it so that it can get the name and the port 
	int file = open(".configure" ,O_RDONLY, 0644);
	int length = lseek(file,0,SEEK_END);

	lseek(file,0, SEEK_SET);
	char * contents = malloc((length)*sizeof(char));
	int size_of_LL;
	node * ptr= NULL;
	node * head = NULL;
	int status = read(file, contents, length);
	int i,j;
    i=0;

    while(i<strlen(contents)){
        if (isalnum(contents[i])){
            j=i;
            while (!iscntrl(contents[i]) && !isspace(contents[i]) && i<strlen(contents)){
                i++;
            }
            char * new_token= malloc(sizeof(char)* (i-j));
            strncpy(new_token,contents+j,i-j);
			ptr=(node*)malloc(sizeof(node));
			ptr->data=new_token;
			ptr->next= NULL; 
			
			if (head==NULL){
				head = ptr;
			
			}else {
				head->next = ptr;
	
			}
		}
		i++;
	}
	close(file);
	return head;
}



int main (int argc, char ** argv){

	if (argc < 3 ){
		printf("you must put in a valid argument\n");
		exit(0);
	}
	node * LL;
	int server;
 	
	if (strcmp(argv[1],"configure")==0){
		if (argc<4){
			printf("you need to put a valid host name and a port number to connect to\n");
			exit(0);
		}
		configure(argv[2],argv[3]);
		LL= tokenize();
	 }
    server = connect_server(argv[2], argv[3]);
	printf("%d\n", server);
	write(server, "coins", 6);
	return 0;
}














































// int main (int argc, char** argv){

// 	node * LL;
// 	if (argv[1]== NULL){
// 		fprintf(stderr,"You must put in an argument for this to run\n");
// 		return 0;
// 	}
// 	if (strcmp(argv[1],"configure")==0){
		
// 		if (argc<4){
// 		fprintf(stderr,"You must put in an IPAddress and a port number \nTerminating\n");
// 		return 0;
// 		}
// 		configue(argv[2],argv[3]);
		
// 		// printf("%s\n", LL->data);
// 		// printf("%lu\n", strlen(LL->next->data));
// 		// printf("%s\n", LL->next->data);
// 	}
// 		LL = tokenize();
// 		// printf("%s\n", LL->data);
// 		// printf("%lu\n", strlen(LL->next->data));
// 		// printf("%s\n", LL->next->data);
// 		int port = atoi(argv[3]);
// 		char * IPaddress = argv[2];
// 	//connect_server()
// 	// this starts the loop of connecting to the server 
// 	// while (1){
// 	// 	// here run the if else for every case of the things we need to do 


// 	// }
// 	return 0;
// }











// int connect_to_server(char* IPAddress, int port){
	
// 	//struct hostent *host_name = gethostbyname (IPAddress);
// 	int soket= 0;
// 	char *hello = "Hello from client"; 	
// 	struct sockaddr_in server_address;
// 	soket = socket(AF_INET, SOCK_STREAM, 0);
// 	if (soket < 0){
// 		fprintf(stderr, "Error %d in creating the socket\n", errno);
// 		exit(0);
// 	} else{
// 		printf("The socket was successfully created \n");
// 	}

// 	memset ( &server_address , '0', sizeof(server_address));
		

// 	server_address.sin_family= AF_INET;

// 	server_address.sin_addr.s_addr= inet_addr(IPAddress);
	
// 	server_address.sin_port = htons(port);
		

// 	if (connect(soket, (struct sockaddr*) &server_address, sizeof(server_address))<0){
// 		fprintf(stderr,"There was a failure connecting to the server\n");
// 		printf("%d\n", errno);
// 		exit(0);
// 	}else{
// 		printf("yay you have successfully connected to the server\n");
	
// 		send(soket , hello , strlen(hello) , 0 ); 
		

//     	printf("Hello message sent\n"); 
// 	}

// 	return soket;

// }
