#pragma once

#define CARD_WIDTH 110
#define CARD_HEIGHT 157

#include <iostream>

#include "heap_monitor.h"
#include "TypesOfCards.h"
#include "TextureManager.h"

using namespace std;

class Card
{
public:
	Card(char cardColor, char cardSymbol, int posX, int posY);     
    ~Card();
	void decryptCard();
	void loadFileCardPicture();
	const char* getFileCardPicture();
	TypesOfCards::Color getCardColor();
	TypesOfCards::Symbol getCardSymbol();
	int getWidth();
	int getHeight();
	int getPosX();
	int getPosY();

private:
	TypesOfCards::Color _cardColor;
	TypesOfCards::Symbol _cardSymbol;
	char _sendCardColor;
	char _sendCardSymbol;

	const char* _fileCardPicture;
	int _width;
	int _height;
	int _posX;
	int _posY;
};