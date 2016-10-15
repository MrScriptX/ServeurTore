#include <iostream>
#include "serveur.h"

int main()
{
	int broadCast{ 0 };
	
	std::cin >> broadCast;

	if (broadCast == 0)
	{
		Server localServ(1111);//local server

		for (int i = 0; i < 100; i++)
		{
			localServ.ListenNewConnections();
		}
	}
	else if (broadCast == 1)
	{
		Server publicServ(1111, true);//public server

		for (int i = 0; i < 100; i++)
		{
			publicServ.ListenNewConnections();
		}
	}

	system("pause");
	return 0;
}