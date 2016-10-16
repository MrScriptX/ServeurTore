#ifndef FILE_DATA_H
#define FILE_DATA_H

#include <fstream>

struct FileTransferData
{
	static const int bufferSize{ 8192 };
	std::string fileName;
	int bytesWritten;
	int fileOffset;
	int fileSize;
	int remainingBytes;
	std::ifstream inStream;
	std::ofstream outStream;
	char buffer[bufferSize];
};

#endif // !file_data.h

