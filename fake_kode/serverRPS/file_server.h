#ifndef __FILE_SERVER_H__
#define __FILE_SERVER_H__

#include <iostream>

class file_server
{
public:
	file_server ();

private:
        void sendFile(std::string fileName, long fileSize, Transport::Transport *myTransport);
	char *buffer;
};

#endif /* __SLIP_SERVER_H__ */
