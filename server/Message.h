#pragma once

#include "Player.h"
#include <string>

class Message {
    Player* _player;
    std::string _message;
public:
    Message(Player* player, char* message);
    Player* getPlayer();
    std::string getMessage();
};