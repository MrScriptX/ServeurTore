#pragma once

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
