#pragma once

#include "Player.h"
#include "Card.h"
#include <vector>
#include <iostream>
#include <algorithm>
#include <random>
#include <chrono>
#include <sys/types.h>
#include <sys/socket.h>

class Room {
    std::vector<Player*> _rivals;
    char _id[2];
    int _state;
    bool _direction;
    int _whosTurn;
    std::vector<Card*> _pile;
    std::vector<Card*> _discardedCards;
public:
    Room(Player* hrac, char *id);
    ~Room();
    void getId(char *id);
    void addPlayer(Player* player);
    int getState();
    void setState(int state);
    std::vector<Player*>* getPlayers();
    void initPile();
    void pileToPile();
    void deletePlayer(Player* player);
    Card* getCard();
    void setDiscard(Card* card);
    bool getDirection();
    void setDirection(bool direction);
    int getTurn();
    void setTurn(int whosTurn);
    bool isPlayerHere(Player* player);
};