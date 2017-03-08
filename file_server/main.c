/*
 ============================================================================
 Name        : file_server.c
 Author      : Lars Mortensen
 Version     : 1.0
 Description : file_server in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iknlib.h>

#define bufflen 255


void sendFile(char *fileName,int outToClient)
{
    while(1)
    {

           /* Open the file that we wish to transfer */
           FILE *fp = fopen(fileName,"r");
           if(fp==NULL)
           {
               printf("File opern error");
               return 1;
           }

           /* Read data from file and send it */
           while(1)
           {
               /* First read file in chunks of 256 bytes */
               unsigned char buff[1000]={0};
               int nread = fread(buff,1,1000,fp);
               printf("Bytes read %d \n", nread);

               /* If read was success, send data. */
               if(nread > 0)
               {
                   printf("Sending \n");
                   write(outToClient, buff, nread);
               }

               /*
                * There is something tricky going on with read ..
                * Either there was error, or we reached end of file.
                */
               if (nread < 1000)
               {
                   if (feof(fp))
                       printf("End of file\n");
                   if (ferror(fp))
                       printf("Error reading\n");
                   close(outToClient);
                   return 0;
                   break;
               }


         }
           printf("Close punkt\n");
           close(outToClient);
    }
     printf("return punkt\n");
return 0;

}

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{

         int sockfd, newsockfd, portno;
         socklen_t clilen;
         char filename[bufflen];
         char buffer[20];
         const char *fileName = &filename;  //Laver en pointer til adressen på filenmae array
         char fsizec[255];

         struct sockaddr_in serv_addr, cli_addr;
         int n;

         if (argc < 2) {
             fprintf(stderr,"ERROR, no port provided\n");
             exit(1);
         }

         sockfd = socket(AF_INET, SOCK_STREAM, 0);
         if (sockfd < 0)
            error("ERROR opening socket");

         bzero((char *) &serv_addr, sizeof(serv_addr));
         portno = atoi(argv[1]);
         serv_addr.sin_family = AF_INET;
         serv_addr.sin_addr.s_addr = INADDR_ANY;
         serv_addr.sin_port = htons(portno);

         if (bind(sockfd, (struct sockaddr *) &serv_addr,
                  sizeof(serv_addr)) < 0)
                  error("ERROR on binding");
         listen(sockfd,5);
         clilen = sizeof(cli_addr);

    while(1)
    {
         newsockfd = accept(sockfd,
                     (struct sockaddr *) &cli_addr,
                     &clilen);
         if (newsockfd < 0)
              error("ERROR on accept");

//---------------------------File check---------------------------



         bzero(filename,bufflen);
         n = read(newsockfd, filename, 255);
         if (n < 0)
              error("ERROR reading from socket");
         filename[n-1] = 0;
         printf("Filnavnet: %s\n",fileName);



         long int file_size;
         file_size = check_File_Exists(filename);
         //printf("File size %d \n",file_size);

         sprintf(fsizec,"%d",file_size); //Laver fsize om til char
         n = write(newsockfd,fsizec,sizeof(fsizec));

         filename[n-1] = 0;
         n = write(newsockfd, filename, n);
         if (n < 0)
              error("ERROR reading from socket");
         filename[n-1] = 0;
         printf("Sending filesize: %d\n",file_size);

        sendFile(filename,newsockfd);



         if(filename == "exit")
         {
             close(newsockfd);
             close(sockfd);
             return 0;
         }
    }

}

