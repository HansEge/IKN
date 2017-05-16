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
    bzero(buffer,BUFSIZE); //Fylder bufferen med 0'er.
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
    ackBuf [SEQNO] = (ackType ? buffer [SEQNO] : (buffer [SEQNO] + 1) % 2) ;
    ackBuf [TYPE] = ACK;
    checksum->calcChecksum (ackBuf, ACKSIZE);

    if(CORRUPT_ACK == 1){
        ackBuf[0] = 0;
        std::cout << "ACK oedelagt" << std::endl;
    }
   CORRUPT_ACK++;

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
void Transport::send(const char buf[], short sizeApp)
{
    int i;

    do{
    i = 0;
    buffer[TYPE] = DATA;
    buffer[SEQNO] = seqNo;

    for(i = 0; i < sizeApp; i++)    //Laegger buf over i buffer
    {
        buffer[i+ACKSIZE] = buf[i];
    }

    checksum->calcChecksum(buffer, sizeApp+ACKSIZE);


    if(CORRUPT_DATA == 1)
    {
        buffer[0] = 0;
    std::cout << "Bit oedelagt" << std::endl;
    }
    link->send(buffer, i+ACKSIZE);




       CORRUPT_DATA++;

    } while(!receiveAck());

}

/// <summary>
/// Receive the specified buffer.
/// </summary>
/// <param name='buffer'>
/// Buffer.
/// </param>
short Transport::receive(char buf[], short sizeApp)
{
    int currentSize = 0;
    bool ACK_;
    int i;

    do{
        currentSize = link->receive(buffer, sizeApp+ACKSIZE);

        ACK_ = checksum->checkChecksum (buffer, currentSize);

        if(ACK_ == false)
            sendAck(false);

        if(buffer[SEQNO] == old_seqNo)  //Registrere hvis vi modtager den samme pakke igen.
        {
            ACK_ = false;   //Koer loop igen
            sendAck(true);
        }



    }while(ACK_ == false);

    sendAck(true);

    old_seqNo = buffer[SEQNO];

    for(i = 0; i < sizeApp && i < (currentSize-ACKSIZE); i++)
    {
        buf[i] = buffer[i+ACKSIZE];
    }

    return i;

}

}

