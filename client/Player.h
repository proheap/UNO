#pragma once

#define CARD_POSX 20
#define CARD_POSY 550

#include <vector>

#include "heap_monitor.h"
#include "Card.h"
#include "WindowObject.h"

using namespace std;

class Player
{
public:
    Player(int order, SDL_Renderer* renderer);
    ~Player();
	void addCard(char cardColor, char cardSymbol);
	void playCard(int order);
	void updateCardPosX();
	vector<Card*> getPlayerHand();
	WindowObject* showCard();

private:
	SDL_Renderer* _renderer;
	vector<Card*> _playerHand;
	vector<WindowObject*> _playerHandObjects;
	int _order;

	int _cardPosX;
	int _cardPosY;
};