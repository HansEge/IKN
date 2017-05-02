#include <fstream>
#include <string>
#include <sstream>
#include <cstring>
#include <cstdlib>
#include "../include/Transport.h"
#include "../include/lib.h"
#include "file_client.h"
#include "../include/Link.h"

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
    /*
    Link::Link * myLink = new Link::Link(BUFSIZE);

    char myMessage[20] = {'hej'};

    mylink->send(myMessage,20);

    //std::cout << "Client message: " << myMessage << std::endl;}
    */
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
void file_client::receiveFile (std::string fileName, Transport::Transport *transport)
{
    //do stuff
}

/// <summary>
/// The entry point of the program, where the program control starts and ends.
/// </summary>
/// <param name='args'>
/// First argument: Filname
/// </param>
int main(int argc, char **argv)
{
    new file_client(argc, argv);

    Link::Link test(1000);
    char buffer[1000] = {};



    test.send(buffer,1000);

    return 0;
}
