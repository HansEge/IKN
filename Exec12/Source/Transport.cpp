#include <iostream>
#include <cstring>
#include <cstdio>
#include "../include/Transport.h"
#include "../include/TransConst.h"

#define DEFAULT_SEQNO 2

namespace Transport
{
	/// <summary>
	/// Initializes a new instance of the <see cref="Transport"/> class.
	/// </summary>
	Transport::Transport (short BUFSIZE)
	{
		link = new Link::Link(BUFSIZE+ACKSIZE);
		checksum = new Checksum();
		buffer = new char[BUFSIZE+ACKSIZE];
		seqNo = 0;
		old_seqNo = DEFAULT_SEQNO;
		errorCount = 0;
        recvSize = 0;
        dataReceived = false;
	}

	/// <summary>
	/// Delete dynamics attribute before delete this object.
	/// </summary>
	Transport::~Transport()
	{
		if(link) delete link;
		if(checksum) delete checksum;
		if(buffer) delete [] buffer;
	}

	/// <summary>
	/// Receives the ack.
	/// </summary>
	/// <returns>
	/// The ack.
	/// </returns>
	bool Transport::receiveAck()
	{
        recvSize = link->receive(buffer, ACKSIZE);
        dataReceived = true;

        if (recvSize == ACKSIZE)
        {
            if(!checksum->checkChecksum(buffer, ACKSIZE) ||
                buffer[SEQNO] != seqNo ||
                buffer[TYPE] != ACK)
            {
                return false;
            }

            seqNo = ((buffer[SEQNO] + 1) % 2);
        }

        return true;
    }

	/// <summary>
	/// Sends the ack.
	/// </summary>
	/// <param name='ackType'>
	/// Ack type.
	/// </param>
	void Transport::sendAck (bool ackType)
	{
		char ackBuf[ACKSIZE];
        ackBuf [SEQNO] = (ackType ? (buffer [SEQNO] + 1) % 2 : buffer [SEQNO]);
        ackBuf [TYPE] = ACK;
		checksum->calcChecksum (ackBuf, ACKSIZE);

		link->send(ackBuf, ACKSIZE);
	}

	/// <summary>
	/// Send the specified buffer and size.
	/// </summary>
	/// <param name='buffer'>
	/// Buffer.
	/// </param>
	/// <param name='size'>
	/// Size.
	/// </param>
	void Transport::send(const char buf[], short size)
	{
		char pakkeBuffer[size+4] = {0};

	do
	{
		pakkeBuffer[TYPE] = DATA; //opbygning af header
		pakkeBuffer[SEQNO] = seqNo; //opbygning af header

		for(int i = 4; i < size; i++)
		{
			pakkeBuffer[i] = buf[i]; //data kopiering
		}
		checksum->calcChecksum(pakkebuffer, ACKSIZE);
		link->send(pakkebuffer, size+4)
	}
	while(!receiveAck())
		old_seqNo = DEFAULT_SEQNO;
	}

  	link->send(pakkeBuffer, size);
	}

	/// <summary>
	/// Receive the specified buffer.
	/// </summary>
	/// <param name='buffer'>
	/// Buffer.
	/// </param>
	short Transport::receive(char buf[], short size)
	{
		int sizeCurrent;

		sizeCurrent = link->receive(buffer, LOOPSIZE+ACKSIZE);

    link->recieve(buf, size);
    if(!checksum->checkChecksum(buffer, ACKSIZE))
    {
        sendAck(false);
    }
		else if () {
			/* code */
		}

	}
}
