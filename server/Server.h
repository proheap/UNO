#pragma once

#include <cstdio> 
#include <cstring> 
#include <cstdlib>
#include <cerrno> 
#include <unistd.h>
#include <vector> 
#include <arpa/inet.h>
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h>
#include "Player.h"
#include <iostream>
#include "Message.h"
#include <mutex>
#include <condition_variable>
#include <fcntl.h>
#include "Room.h"

#define PORT 5555
#define MAXCLIENTS 30

class Server {
    struct sockaddr_in _address;
    int _port;
    int _mainSocket;
    std::vector<Player*> _players;
    std::vector<Message*> _messages;
    std::vector<Room*> _rooms;
    std::mutex _mutex;
    std::condition_variable _newMessage;
public:
    Server(int port);
    ~Server();
    void init();
    void recvCycle();
    void sendCycle();
    int findRoom(char *id);
    int findPlayer(char *id);
    void deleteRoom(Room* room);
};