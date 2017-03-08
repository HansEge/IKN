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

void receiveFile(char *fileName, int socketfd)
{
/* Create file where data will be stored */
    FILE *fp;
    long int bytesReceived = 0;
    int count = 0;
    char recvBuff[1000];
    fp = fopen(fileName, "w");
    if(NULL == fp)
    {
        printf("Error opening file");
        return 1;
    }
     bzero(recvBuff,1000);

    /* Receive data in chunks of 1000 bytes */
    while((bytesReceived = read(socketfd, recvBuff, 1000)) > 0)
    {


        if(count == 1)
        {

            printf("Bytes received %d\n",bytesReceived);
            // recvBuff[n] = 0;
            fwrite(recvBuff, 1,bytesReceived,fp);
            // printf("%s \n", recvBuff);
        }
        if(bytesReceived == 255)
            count = 1;

    }

    if(bytesReceived < 0)
    {
        printf("\n Read Error \n");
    }
count = 0;

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
    long int sizeOfFile;
    long int fsize;
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
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");

    
//----------------------------------FIND FILNAVN-------------------------------

    printf("Indtast filnavn: ");
    bzero(buffer,256);
    fgets(buffer,255,stdin);
    n = write(sockfd,buffer, strlen(buffer));

    int i = 0;
    for(i = 0; i < 256; i++)
      {
        fileName[i] = buffer[i];
      }
//----------------------------------END OF FIND FILNAVN------------------------




//----------------------------------KONVERTER TIL FILSTØRRELSE------------------------
    bzero(buffer,256);
    sizeOfFile = read(sockfd,buffer,255);
    fsize = atol(buffer);
    printf("Filesize %ld \n",fsize);
//----------------------------------END OF KONVERTER TIL FILSTØRRELSE-------------------
    fileName[n-1] = 0;
    receiveFile(fileName,sockfd);
    close(sockfd);
    return 0;
}




/**
 * Modtager filstørrelsen og udskriver meddelelsen: "Filen findes ikke" hvis størrelsen = 0
 * ellers
 * åbnes filen for at skrive de bytes som senere modtages fra serveren (HUSK kun selve filnavn)
 * Modtag og gem filen i blokke af 1000 bytes indtil alle bytes er modtaget.
 * Luk filen, samt input output streams
 *
 * @param fileName Det fulde filnavn incl. evt. stinavn
 * @param sockfd Stream for at skrive til/læse fra serveren
 */
