#include <iostream>
#include "serveur.h"

int main(int argc, char *argv[])
{
	
	Server MyServer(1111); //Create server on port 100
	for (int i = 0; i < 100; i++) //Up to 100 times...
	{
		MyServer.ListenNewConnections(); //Accept new connection (if someones trying to connect)
	}
	system("pause");
	return 0;
}