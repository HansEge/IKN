#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstring>
#include "../include/Transport.h"
#include "../include/lib.h"
#include "file_server.h"

/// <summary>
/// The BUFSIZE
/// </summary>
#define BUFSIZE 1000

/// <summary>
/// Initializes a new instance of the <see cref="file_server"/> class.
/// </summary>
file_server::file_server ()
{
    Transport::Transport * myTransport = new Transport::Transport(BUFSIZE);

    char myMessage[] = "JALLA";

    myTransport->send(myMessage,strlen(myMessage));

}

/// <summary>
/// Sends the file.
/// </summary>
/// <param name='fileName'>
/// File name.
/// </param>
/// <param name='fileSize'>
/// File size.
/// </param>
/// <param name='transport'>
/// Transport lag.
/// </param>
void file_server::sendFile(std::string fileName, long fileSize, Transport::Transport *transport)
{


    while(1)
        {
               long int rest = fileSize;

               /* Open the file that we wish to transfer */
               FILE *fp = fopen("test.txt","r");
               if(fp==NULL)
               {
                   printf("File opern error");
                   //return 0;
               }

               /* Read data from file and send it */

              do
              {
                   /* First read file in chunks of 1000 bytes */
                   char buff[1000]={0};
                   int nread = fread(buff,1,1000,fp);
                   printf("Bytes read %d \n", nread);


                   printf("Sending \n");
                   transport->send(buff,nread);

                   rest -= nread;

               }
               while(rest > 0);

               printf("Send succesfull!\n");

               return;

        }


}

/// <summary>
/// The entry point of the program, where the program control starts and ends.
/// </summary>
/// <param name='args'>
/// The command-line arguments.
/// </param>
int main(int argc, char **argv)
{
	new file_server();

	return 0;
}
