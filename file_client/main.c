/*
 ============================================================================
 Name        : file_client.c
 Author      : Lars Mortensen
 Version     : 1.0
 Description : file_client in C, Ansi-style
 ============================================================================
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <iknlib.h>

void receiveFile(char *fileName,long int fileSize, int socketfd)
{
/* Create file where data will be stored */
    FILE *fp;
    int n;
    long int bytesReceived = 0;
    int count = 0;
    char recvBuff[1000];

    n = strlen(fileName);

    fileName[n-1] = 0;

    fp = fopen(fileName, "w");
    if(NULL == fp)
    {
        printf("Error opening file");
        return 1;
    }
     bzero(recvBuff,1000);

     long int rest = fileSize;

    /* Receive data in chunks of 1000 bytes */
     do
     {
        bytesReceived = read(socketfd, recvBuff, 1000);
        if(count == 1)
        {
            printf("Bytes received %d\n",bytesReceived);
            n = fwrite(recvBuff, 1,bytesReceived,fp);
            rest -= n;
        }
        if(bytesReceived == 504)
        	count = 1;
     }
     while(rest > 0);

     printf("Filetransfer complete!\n");
     return 0;
}


void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    char fileName[256];
    long int fileSize;
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[256];
    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,server->h_length);
    serv_addr.sin_port = htons(portno);

//----------------------------------KODE VI HAR LAVET STARTER HER-----------------------------------

    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
            error("ERROR connecting");

        /*Skriver filepath til serveren*/
        printf("Please enter the message: ");
        bzero(buffer,256); //Tømmer buffer
        fgets(buffer,255,stdin); //Fylder buffer med brugerens input

        n = write(sockfd,buffer,strlen(buffer)); //Sender bufferens indhold til serveren
        if (n < 0)
            error("ERROR writing to socket");
        fileSize = getFileSizeTCP(sockfd);
        printf("Filesize: %d\n",fileSize);

        bcopy(extractFileName(buffer), fileName, sizeof(buffer)); //Finder filnavn

        printf("Filename: %s\n",fileName);

        /*Modtagelse af fil*/
        receiveFile(fileName,fileSize,sockfd);

        close(sockfd);

        return 0;
}
