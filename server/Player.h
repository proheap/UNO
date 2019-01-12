#pragma once

#include <iostream>
#include <vector>
#include "Card.h"

class Player {
    int _sock;
    void* _room;
    bool _uno;
    bool _roundPassed;
    std::vector<Card*> _hand;
public:
    Player(int sock);
    ~Player();
    int getSock();
    void setRoom(void* room);
    void* getRoom();
    void addCard(Card* card);
    void clearHand();
    int getCardsNumber();
    Card* getCard(int i);
    bool getUno();
    void setUno(bool uno);
    void setRoundPassed(bool roundPassed);
    bool getRoundPassed();
};