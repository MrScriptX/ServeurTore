#include "client.h"

Client::Client(std::string IP, int port)
{
	WSADATA wsaData;
	WORD dllVersion = MAKEWORD(2, 2);

	if (WSAStartup(dllVersion, &wsaData) != 0)//starting winsock and check sucessfull startup
	{
		MessageBoxA(NULL, "Winsock Startup failed", "Error", MB_OK | MB_ICONERROR);
		exit(1);
	}

	m_socket = socket(AF_INET, SOCK_STREAM, NULL);

	m_addr.sin_addr.s_addr = inet_addr(IP.c_str());
	m_addr.sin_family = AF_INET;
	m_addr.sin_port = htons(port);

	connect(m_socket, (struct sockaddr*)&m_addr, sizeof(m_addr) < 0);
}