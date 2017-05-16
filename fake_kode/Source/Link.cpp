#include <string.h>
#include <strings.h>
#include <iostream>
#include <cstdio>
#include "../include/Link.h"

using namespace std;

namespace Link {

/**
  * Explicitet constructor for linklaget.
  * Åbner den serielle port og sætter baudraten, paritet og antal stopbits
  */
Link::Link(int bufsize)
{
    buffer = new char[bufsize*2+2];

    serialPort=v24OpenPort("/dev/ttyS1",V24_STANDARD);
    if ( serialPort==NULL )
    {
        fputs("error: sorry, open failed!\n",stderr);
        exit(1);
    }

    int rc=v24SetParameters(serialPort,V24_B115200,V24_8BIT,V24_NONE);
    if ( rc!=V24_E_OK )
    {
        fputs("error: setup of the port failed!\n",stderr);
        v24ClosePort(serialPort);
        exit(1);
    }

    // Uncomment following lines to use timeout
/*    rc=v24SetTimeouts(serialPort,5);
    if ( rc!=V24_E_OK )
    {
        fputs("error: setup of the port timeout failed!\n",stderr);
        v24ClosePort(serialPort);
        exit(1);
    }

    rc=v24FlushRxQueue(serialPort);
    if ( rc!= V24_E_OK )
    {
    	fputs("error: flushing receiverqueue\n", stderr);
    	v24ClosePort(serialPort);
    	exit(1);
    }

    rc=v24FlushTxQueue(serialPort);
    if ( rc!= V24_E_OK )
    {
    	fputs("error: flushing transmitterqueue\n", stderr);
    	v24ClosePort(serialPort);
    	exit(1);
    }
*/
}

/**
  * Destructor for linklaget.
  * Lukker den serielle port
  */
Link::~Link()
{
	if(serialPort != NULL)
		v24ClosePort(serialPort);
	if(buffer != NULL)
		delete [] buffer;
}

/**
 * Bytestuffer buffer, dog maximalt det antal som angives af size
 * Herefter sendes de til modtageren via den serielle port
 *
 * @param buffer De bytes som skal sendes
 * @param size Antal bytes der skal sendes
 */
void Link::send(const char buf[], short size)
{
    int i;
    int j = 0;

    buffer[j] = 'A';

    for (i = 0; i < size; i++)
    {
        j++;
		switch(buf[i])
		{
			case 'A':
				buffer[j] = 'B';
				j++;
				buffer[j] = 'C';
                //size++;
				break;

			case 'B':
				buffer[j] = 'B';
				j++;
				buffer[j] = 'D';
                //size++;
				break;

			default:
				buffer[j] = buf[i];
        }

    }

    buffer[j+1] = 'A';
    v24Write (serialPort, (unsigned char *)buffer, j+2);



}

/**
 * Modtager data fra den serielle port og debytestuffer data og gemmer disse i buffer.
 * Modtager højst det antal bytes som angives af size.
 * Husk kun at læse en byte ad gangen fra den serielle port.
 *
 * @param buffer De bytes som modtages efter debytestuffing
 * @param size Antal bytes der maximalt kan modtages
 * @return Antal bytes modtaget
 */
short Link::receive(char buf[], short size)
{
    do{
        buffer[0] = v24Getc(serialPort);
    } while(buffer[0] != 'A'); //Venter på et A

    /*Fylder lokal buffer med data fra seriel port*/
    for (int i = 1; i <= size*2+2; i++)
        {
            buffer[i] = v24Getc(serialPort);

            if (buffer[i] == 'A')
                break;
        }

    /*Dekrypterer lokal buffer og lægger det over i buf*/
    bool BREAK = false;
    int j = 0;
    int i;

    for (i = 1; j < size && i < size*2+2; i++) //Vi starter ved i=1 fordi vi ved der ligger 'A' på første plads
    {
        switch(buffer[i])
        {
            case 'A':
                BREAK = true;
                break;

            case 'B':
                switch(buffer[i+1])
                {
                    case 'C':
                        buf[j] = 'A';
                        i++;
                        break;

                    case 'D':
                        buf[j] = 'B';
                        i++;
                        break;

                    default:
                        buf[j] = 'B';
                        break;
                }
                break;

            default:
                buf[j] = buffer[i];
                break;
        }

        if (BREAK)
            break;
        
        j++;
    }



    return j;
}


} /* namespace Link */
