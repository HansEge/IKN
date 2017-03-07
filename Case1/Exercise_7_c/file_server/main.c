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


void sendFile(char *fileName, long fileSize, int outToClient);

/*
 * Opretter en socket.
 * Venter på en connect fra en klient.
 * Modtager filnavn
 * Finder filstørrelsen
 * Kalder metoden sendFile
 * Lukker socketen og programmet
 *
 * @param argc Antal argumenter
 * @param argv Argumenterne i et array
 */

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
         const char *fileName = &filename;  //Laver en pointer til adressen på filenmae array

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
         newsockfd = accept(sockfd,
                     (struct sockaddr *) &cli_addr,
                     &clilen);
         if (newsockfd < 0)
              error("ERROR on accept");

//---------------------------File check---------------------------



         bzero(filename,bufflen);
        // fileName = readTextTCP(filename, sizeof(filename), newsockfd);
         n = read(newsockfd, filename, 255);
         if (n < 0)
              error("ERROR reading from socket");
         filename[n-1] = 0;
         printf("Filnavnet: %s\n",fileName);



         long int file_size;
         file_size = check_File_Exists(filename);
         printf("File size %d \n",file_size);

         n = write(newsockfd, filename, n);
         if (n < 0)
              error("ERROR reading from socket");
         filename[n-1] = 0;
         printf("Sending filesize: %d\n",file_size);




         close(newsockfd);
         close(sockfd);
         return 0;
}


/*
 * Sender filstørrelsen
 * Hvis filstørrelsen = 0 sendes indhold af filen ikke
 * ellers
 * åbnes filen for at læse den og sende indhold i blokke af 1000 bytes til klienten.
 * Luk filen.
 *
 * @param fileName Det fulde filnavn incl. evt. stinavn.
 * @param fileSize Størrelsen på filen.
 * @param outToClient Stream for at skrive til serveren.
 */
void sendFile(char *fileName, long fileSize, int outToClient)
{
    // TO DO Your own code
}
