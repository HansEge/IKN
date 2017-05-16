#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstring>
#include "../include/Transport.h"
#include "../include/lib.h"
#include "file_server.h"
#include <sys/stat.h>   //Til at finde filstørrelse
#include <fcntl.h>
#include <unistd.h>

using namespace std;


short CORRUPT_DATA;
short CORRUPT_ACK;

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
    char buffer[BUFSIZE] = {0};
    long fsize;


    CORRUPT_DATA = 2;
    CORRUPT_ACK = 1;

    /*Modtager filnavn*/
    myTransport->receive(buffer,BUFSIZE);
    std::cout << "Forspurgte fil: " << buffer << std::endl;
    string fileName_(buffer);

    /*Soeger efter filen*/
    struct stat sts;
    if ((stat (buffer, &sts)) == -1)
    {
        fprintf(stderr,"Fejl: Filen findes ikke. \n");
        fsize = 0;
    }
    else
    {
        fsize = sts.st_size;
        printf("Fil stoerrelse: %d \n", fsize);
    }

    /*Sender storelsen på filen*/
    sprintf(buffer,"%d",fsize);
    myTransport->send(buffer, sizeof(buffer));

    if(fsize == 0)
    {
        exit(0);
    }


    /*Sender fil*/
    sendFile(fileName_,fsize,myTransport);

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
void file_server::sendFile(std::string fileName, long fileSize, Transport::Transport *myTransport)
{
    int fd;
    char fbuffer[BUFSIZE];

    /*Åbner file descriptor*/
    fd = open(fileName.c_str(), O_RDONLY);
    if (fd == -1)
    {
        error ("Fejl: Kan ikke aabne filen");
        exit(0);
    }
    printf("File descriptor: %d \n", fd);

    /*Sender filen*/
    for (int i = 0; i<fileSize/BUFSIZE+1; i++)
    {
        if (i<fileSize/BUFSIZE)
        {
            read(fd, fbuffer, BUFSIZE);
            myTransport->send(fbuffer,BUFSIZE);
        }
        else
        {
            read(fd, fbuffer, fileSize%BUFSIZE);
            myTransport->send(fbuffer,fileSize%BUFSIZE);
        }

    }

    close(fd);
}

/// <summary>
/// The entry point of the program, where the program control starts and ends.
/// </summary>
/// <param name='args'>
/// The command-line arguments.
/// </param>
int main(int argc, char **argv)
{

    file_server* myServer = new file_server();

    return 0;
}

















