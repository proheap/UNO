#pragma once

#define FILE_DECK "assets/cards/back.png"

#include <vector>

#include "heap_monitor.h"
#include "Card.h"
#include "WindowObject.h"

using namespace std;

class Table
{
public:
    Table(SDL_Renderer* renderer);
    ~Table();
    void setCard(Card* card);
	int getCardPosX();
	int getCardPosY();
	Card* getPlayedCard();
	WindowObject* getDeckObject();
	WindowObject* getPlayedCardObject();

private:
	SDL_Renderer* _renderer;
	WindowObject* _deckObject;
	vector<Card*> _playedCards;
	vector<WindowObject*> _playedCardsObjects;
	int _cardPosX;
	int _cardPosY;
	int _deckPosX;
	int _deckPosY;
};