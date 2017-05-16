#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstring>
#include <cstdlib>
#include "../include/Transport.h"
#include "../include/lib.h"
#include "file_client.h"
#include <fcntl.h>      //Til at åbne filer
#include <unistd.h>

using namespace std;


short CORRUPT_DATA;
short CORRUPT_ACK;

/// <summary>
/// The BUFSIZE
/// </summary>
#define BUFSIZE 1000

/// <summary>
/// Initializes a new instance of the <see cref="file_client"/> class.
///
/// file_client metoden opretter en peer-to-peer forbindelse
/// Sender en forspørgsel for en bestemt fil om denne findes på serveren
/// Modtager filen hvis denne findes eller en besked om at den ikke findes (jvf. protokol beskrivelse)
/// Lukker alle streams og den modtagede fil
/// Udskriver en fejl-meddelelse hvis ikke antal argumenter er rigtige
/// </summary>
/// <param name='args'>
/// Filnavn med evtuelle sti.
/// </param>
file_client::file_client(int argc, char **argv)
{
    Transport::Transport * myTransport = new Transport::Transport(BUFSIZE);
    char buffer[BUFSIZE] = {0};

    CORRUPT_DATA = 2; //Hvis det er mere end 1 så laves der ikke corrupt
    CORRUPT_ACK = 2; //Hvis det er mere end 1 så laves der ikke corrupt

    /*Sender filnavn*/
    myTransport->send(argv[1],strlen(argv[1]));

    /*Modtager fil*/
    string fileName(argv[1]);
    receiveFile(fileName,myTransport);

}

/// <summary>
/// Receives the file.
/// </summary>
/// <param name='fileName'>
/// File name.
/// </param>
/// <param name='transport'>
/// Transportlaget
/// </param>
void file_client::receiveFile (std::string fileName, Transport::Transport *myTransport)
{
    char buffer[BUFSIZE] = {0};
    int n, recievedSize = 0;
    long fsize;
    int fd;

    /*Filstørrelse*/
    myTransport->receive(buffer,BUFSIZE);
    fsize = atol(buffer);
    printf("Fil stoerrelse: %d \n", fsize);

    if (fsize == 0)
    {
        fprintf(stderr,"ERROR: Filen findes ikke. \n");
        exit(0);
    }

    /*Laver file descriptor*/
    fd = open(fileName.c_str(), O_WRONLY | O_CREAT,S_IXGRP);
    if (fd == -1) error ("Kunne ikke aabne fil");
    printf("File descriptor: %d \n", fd);

    /*Fil fra serveren*/
    for (int i = 0; i<fsize/BUFSIZE+1; i++)
    {

        if (i<fsize/BUFSIZE) //Hvis der er mere end 1000bit tilbage
        {
            recievedSize = myTransport->receive(buffer,BUFSIZE);
            n = write(fd, buffer, BUFSIZE); /*fil bliver lagt over i fd*/

        }

        else
        {
            recievedSize = myTransport->receive(buffer,fsize%BUFSIZE);
            n = write(fd, buffer, fsize%BUFSIZE); /*fil bliver lagt over i fd*/

            printf("Sidste bit er modtaget: %d \n", recievedSize);

        }


    }

    close(fd);





}

/// <summary>
/// The entry point of the program, where the program control starts and ends.
/// </summary>
/// <param name='args'>
/// First argument: Filname
/// </param>
int main(int argc, char** argv)
{
    if (argc < 2)
    {
       fprintf(stderr,"FEJL: Skriv navnet på filen igen \n", argv[0]);
       exit(0);
    }

    file_client* myClient = new file_client(argc, argv);

	return 0;
}
