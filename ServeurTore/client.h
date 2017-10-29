#pragma once

#include "ressource.h"

class Client
{
public:
	Client(std::string IP, int port);
	
	void Send_PacketType(PacketType _packet);
	void Get_PacketType(PacketType _packet);
	void Send_message(std::string msg);
	void Send_Int32_t(int32_t _int);


private:
	SOCKADDR_IN m_addr;
	SOCKET m_socket;

};
