#include "Message.h"

Message::Message(Player* player, char* message) {
    _player = player;
    _message = message;
}

Player* Message::getPlayer() {
    return _player;
}

std::string Message::getMessage() {
    return _message;
}