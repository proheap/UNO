#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib,"ws2_32.lib")
#include <WinSock2.h>
#include <iostream>
#include <string>
#include <vector>
#include "heap_monitor.h"

using namespace std;

class Client
{
public:
	Client(char* ip, int port);
	~Client();
	SOCKET getSocket();

private:
	SOCKET _serverSocket;
};

