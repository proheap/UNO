#include <cstdio>
#include "Server.h"
#include <cstdlib>
#include <ctime>
#include <thread>
#include <iostream>

int main() {
    srand(time(NULL));
    Server *server = new Server(5555);
    std::thread sender(&Server::sendCycle, server);
    server->recvCycle();
    sender.join();
    delete server;
    server = nullptr;
    return 0;
}