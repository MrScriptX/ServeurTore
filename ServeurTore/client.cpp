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

	if (connect(m_socket, (struct sockaddr*)&m_addr, sizeof(m_addr) < 0) == SOCKET_ERROR)
	{
		//error
	}
}

bool Client::ProcessPacket(int ID, PacketType _packetType)
{
	switch (_packetType)
	{
	case PacketType::ChatMessage:
	{
		std::string Message;
		if (!GetString(ID, Message))
			return false;

		for (int i = 0; i < m_totalConnections; i++)
		{
			if (i == ID)
				continue;

			if (!SendString(i, Message))
			{
				std::clog << "Failed to send message from client ID: " << ID << " to client ID: " << i << std::endl;
			}
		}
		std::clog << "Processed chat message packet from user ID: " << ID << std::endl;
		break;
	}
	case PacketType::TransfertFile_Request:
	{
		std::string fileName;
		if (!GetString(ID, fileName))
		{
			return false;
		}
		m_connections[ID].file.inStream.open(fileName, std::ios::binary | std::ios::ate);

		if (!m_connections[ID].file.inStream.is_open())
		{
			std::cout << "Client " << ID << "requested file: " << fileName << ", but that file didn't open";
			std::string ErrMsg = "RequestFile: " + fileName + " does not exist";
			if (!SendString(ID, ErrMsg))
			{
				return false;
			}

			return true;
		}

		m_connections[ID].file.fileName = fileName;
		m_connections[ID].file.fileSize = m_connections[ID].file.inStream.tellg();
		m_connections[ID].file.inStream.seekg(0);
		m_connections[ID].file.fileOffset = 0;

		if (!HandleSendFile(ID))
		{
			return false;
		}
		break;
	}
	case PacketType::TransferFile_NextBuffer:
	{
		if (!HandleSendFile(ID))
			return false;

		break;
	}

	default:
	{
		std::clog << "Unrecognized packet:" << (int32_t)_packetType << std::endl;
		break;
	}
	}
	return true;
}

bool Client::HandleSendFile(int ID)
{
	if (m_connections[ID].file.fileOffset >= m_connections[ID].file.fileSize)
		return true;

	if (!SendPacketType(ID, PacketType::TransferFile_ByteBuffer))
		return false;

	m_connections[ID].file.remainingBytes = m_connections[ID].file.fileSize - m_connections[ID].file.fileOffset;

	if (m_connections[ID].file.remainingBytes > m_connections[ID].file.bufferSize)
	{
		m_connections[ID].file.inStream.read(m_connections[ID].file.buffer, m_connections[ID].file.bufferSize);
		if (!SendInt32_t(ID, m_connections[ID].file.bufferSize))
			return false;

		if (!SendAll(ID, m_connections[ID].file.buffer, m_connections[ID].file.bufferSize))
			return false;
	}
	else
	{
		m_connections[ID].file.inStream.read(m_connections[ID].file.buffer, m_connections[ID].file.remainingBytes);
		if (!SendInt32_t(ID, m_connections[ID].file.remainingBytes))
			return false;

		if (!SendAll(ID, m_connections[ID].file.buffer, m_connections[ID].file.remainingBytes))
			return false;

		m_connections[ID].file.fileOffset += m_connections[ID].file.remainingBytes;
	}

	if (m_connections[ID].file.fileOffset == m_connections[ID].file.fileSize)
	{
		if (!SendPacketType(ID, PacketType::TransferFile_EndFile))
			return false;

		std::cout << "File sent: " << m_connections[ID].file.fileName << std::endl;
		std::cout << "File size(bytes): " << m_connections[ID].file.fileSize << std::endl << std::endl;

		m_connections[ID].file.inStream.close();
	}

	return true;
}


bool Client::SendAll(int ID, char* data, int totalBytes)
{
	int bytesSend{ 0 };

	while (bytesSend < totalBytes)
	{
		int returnCheck = send(m_connections[ID].socket, data + bytesSend, totalBytes - bytesSend, NULL);
		if (returnCheck == SOCKET_ERROR)
			return false;

		bytesSend += returnCheck;
	}

	return true;
}

bool Client::RecvAll(int ID, char* data, int totalBytes)
{
	int bytesRecv{ 0 };

	while (bytesRecv < totalBytes)
	{
		int returnCheck = recv(m_connections[ID].socket, data + bytesRecv, totalBytes - bytesRecv, NULL);
		if (returnCheck == SOCKET_ERROR)
			return false;

		bytesRecv += returnCheck;
	}

	return true;
}

bool Client::SendInt32_t(int ID, int32_t _int)
{
	_int = htonl(_int);

	if (!SendAll(ID, (char*)&_int, sizeof(int32_t)))
		return false;

	return true;
}

bool Client::GetInt32_t(int ID, int32_t &_int)
{
	if (!RecvAll(ID, (char*)&_int, sizeof(int32_t)))
		return false;

	_int = ntohl(_int);
	return true;
}

bool Client::SendString(int ID, std::string &_string)
{
	if (!SendPacketType(ID, PacketType::ChatMessage))
		return false;

	int32_t bufferlength = _string.size();

	if (!SendInt32_t(ID, bufferlength))
		return false;

	if (!SendAll(ID, (char*)_string.c_str(), bufferlength))
		return false;

	return true;
}

bool Client::GetString(int ID, std::string &_string)
{
	int32_t bufferlength; //Holds length of the message

	if (!GetInt32_t(ID, bufferlength)) //Get length of buffer and store it in variable: bufferlength
		return false; //If get int fails, return false

	char * buffer = new char[bufferlength + 1]; //Allocate buffer
	buffer[bufferlength] = '\0'; //Set last character of buffer to be a null terminator so we aren't printing memory that we shouldn't be looking at

	if (!RecvAll(ID, buffer, bufferlength))
	{
		delete[] buffer;
		return false;
	}

	_string = buffer; //set string to received buffer message
	delete[] buffer; //Deallocate buffer memory (cleanup to prevent memory leak)
	return true;//Return true if we were successful in retrieving the string
}

bool Client::SendPacketType(int ID, PacketType _packetType)
{
	if (!SendInt32_t(ID, (int32_t)_packetType))
		return false;

	return true;
}

bool Client::GetPacketType(int ID, PacketType &_packetType)
{
	int packetType;

	if (!GetInt32_t(ID, packetType))
		return false;

	_packetType = (PacketType)packetType;
	return true;
}