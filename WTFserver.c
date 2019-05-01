#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

void error(const char *msg)
{
    perror(msg);
    exit(0);
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
        printf("receive message:%s\n", buffer);
        return 0;
        if (strcmp(buffer, "done") == 0)
            break;
    }
    close(*fd);
    return NULL;
}

int main(int argc, char *argv[])
{
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
