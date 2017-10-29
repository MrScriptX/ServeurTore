#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <string>
#include "WinSock2.h"

#pragma comment(lib,"ws2_32.lib") 

class Client
{
public:
	Client(std::string IP, int port);


private:
	SOCKADDR_IN m_addr;
	SOCKET m_socket;

};
