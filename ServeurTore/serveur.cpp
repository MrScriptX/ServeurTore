#include "serveur.h"


Server::Server(int PORT, bool broadcastPublic)
{
	WSADATA wsaData;
	WORD dllVersion = MAKEWORD(2, 1);

	if (WSAStartup(dllVersion, &wsaData) != 0)//starting winsock and check sucessfull startup
	{
		MessageBoxA(NULL, "Winsock Startup failed", "Error", MB_OK | MB_ICONERROR);
		exit(1);
	}

	if (broadcastPublic == true)//check if server open to public
	{
		m_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	}
	else//server open to local only
	{
		m_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	}

	m_addr.sin_port = htons(PORT);//port used
	m_addr.sin_family = AF_INET;//get IPv4 connections
	m_sockListen = socket(AF_INET, SOCK_STREAM, NULL);//creating listening socket

	if (bind(m_sockListen, (SOCKADDR*)&m_addr, sizeof(m_addr)) == SOCKET_ERROR)//bind the port to socket
	{
		std::string errorMsg{ "Failed to bind to our listening port. Winsock error: " + std::to_string(WSAGetLastError()) };
		MessageBoxA(NULL, errorMsg.c_str(), "Error", MB_OK | MB_ICONERROR);
		exit(1);
	}

	if (listen(m_sockListen, SOMAXCONN) == SOCKET_ERROR)
	{
		std::string errorMsg{ "Listening socket failed. Winsock error: " + std::to_string(WSAGetLastError()) };
		MessageBoxA(NULL, errorMsg.c_str(), "Error", MB_OK | MB_ICONERROR);
		exit(1);
	}

	serverPtr = this;
}

bool Server::ListenNewConnections()
{
	return true;
}

void Server::ClientHandlerThread()
{

}