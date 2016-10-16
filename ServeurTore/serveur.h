#ifndef SERVEUR_H
#define SERVEUR_H
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
	
	bool SendAll(int ID, char* data, int totalBytes);
	bool RecvAll(int ID, char* data, int totalBytes);
	bool SendInt32_t(int ID, int32_t _int);
	bool GetInt32_t(int ID, int32_t &_int);
	bool SendPacketType(int ID, PacketType _packetType);
	bool GetPacketType(int ID, PacketType &_packetType);
	bool SendString(int ID, std::string &_string);
	bool GetString(int ID, std::string &_string);
	
	bool ProcessPacket(int ID, PacketType _packetType);
	bool HandleSendFile(int ID);
	static void ClientHandlerThread(int ID);

	Connection m_connections[100];
	int m_totalConnections = 0;
	SOCKADDR_IN m_addr;
	int m_addrSize = sizeof(m_addr);
	SOCKET m_sockListen;
};

static Server*  serverPtr;

#endif // !serveur.h

