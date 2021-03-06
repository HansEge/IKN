#include <cstdio>
#include "../include/Link.h"
#include <string.h>
#include <iostream>
#include <stdio.h>

namespace Link {

/**
  * Explicitet constructor for linklaget.
  * Åbner den serielle port og sætter baudraten, paritet og antal stopbits
  */
Link::Link(int bufsize)
{
    buffer = new char[( bufsize*2)];

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
	int j = 0;
	buffer[0] = 'A';
    int Acount = 1;

        for(int i = 0; i < size-1; ++i)
		{
			++j;
			if(buf[i] == 'A')
			{
				buffer[j] = 'B';
				++j;
				buffer[j] = 'C';
			}
			else if(buf[i] == 'B')
			{
				buffer[j] = 'B';
				++j;
                buffer[j] = 'D';
			}
            else if (buf[i] == 0 && Acount > 0)
            {
                buffer[j] = 'A';
                Acount = 0;
            }
			else
			buffer[j] = buf[i];
		}
        v24Write (serialPort, (unsigned char *)buffer, strlen(buffer));
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
	int j = 0;

	if (v24Getc(serialPort) != 'A')
	return 0;

	for(int i = 1; i < size; i++)
	{
		buffer[i] = v24Getc(serialPort);
		if(buffer[i] == 'A')
		break;
	}


		for(int i = 1; i < size-1; ++i)
		{
			if(buffer[i] == 'B' && buffer[i+1] == 'C')
			{
				buf[j] = 'A';
				++i;
			}
			if(buffer[i] == 'B' && buffer[i] == 'D')
			{
				buf[j] = 'B';
				++i;
			}
			else
			buf[j] = buffer[i];
			++j;
		}
        std::cout << buffer << std::endl;
        return j;

}
} /* namespace Link */
