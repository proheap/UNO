#include "Client.h"

Client::Client(char* ip, int port)
{
	WSAData wsaData;
	WORD DllVersion = MAKEWORD(2, 1);
	if (WSAStartup(DllVersion, &wsaData) != 0)
	{
		MessageBoxA(NULL, "Winsock startup failed", "Error", MB_OK | MB_ICONERROR);
		exit(1);
	}

	SOCKADDR_IN addr;
	int sizeofaddr = sizeof(addr);
	addr.sin_addr.s_addr = inet_addr(ip);
	addr.sin_port = htons(port);
	addr.sin_family = AF_INET;

	_serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (connect(_serverSocket, (SOCKADDR*)&addr, sizeofaddr) != 0)
	{
		MessageBoxA(NULL, "Failed to Connect", "Error", MB_OK | MB_ICONERROR);
		exit(1);
	}
}

Client::~Client()
{
	shutdown(_serverSocket, SD_SEND);
	closesocket(_serverSocket);
}

SOCKET Client::getSocket()
{
	return _serverSocket;
}