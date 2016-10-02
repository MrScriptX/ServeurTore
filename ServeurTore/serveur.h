#ifndef serveur.h
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#pragma comment(lib, "ws2_32.lib")
#include <string>
#include <iostream>
#include "WinSock2.h"
#include "file_data.h"

enum class PacketType
{
	ChatMessage,
	TransfertFile_Request,
	TransferFile_EndFile,
	TransferFile_ByteBuffer,
	TransferFile_NextBuffer
};

struct Connection
{
	SOCKET socket;
	FileTransferData file;
};

class Server
{
public:
	Server(int PORT, bool broadcastPublic = false);
	bool ListenNewConnections();

private:
	void ClientHandlerThread();

	Connection m_connections[100];
	int m_totalConnections = 0;
	SOCKADDR_IN m_addr;
	int m_addrSize = sizeof(m_addr);
	SOCKET m_sockListen;
};

static Server*  serverPtr;

#endif // !serveur.h

