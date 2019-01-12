#pragma once

#define FILE_OPPONENT "assets/opponent.png"
#define FILE_BACK_CARD "assets/cards/back.png"
#define FILE_BACKSIDE_CARD "assets/cards/back_side.png"
#define OPPONENT_WIDTH 100
#define OPPONENT_HEIGHT 100
#define BACK_CARD_WIDTH 110
#define BACK_CARD_HEIGHT 157
#define SIDE_CARD_WIDTH 157
#define SIDE_CARD_HEIGHT 110

#include "heap_monitor.h"

class Opponent
{
public:
	Opponent(int order, int playerOrder);
	~Opponent();
	void setPosition();
	void setFileOpponentCard();
	void setCardsCount(int count);
	int getCardsCount();
	int getOrder();
	int getPosX();
	int getPosY();
	int getChangeCardPos();
	void setCardPos(int cardPos);
	void setDefaultCardPos();
	int getCardPosX();
	int getCardPosY();
	int getWidth();
	int getHeight();
	int getCardWidth();
	int getCardHeight();
	const char* getFileOpponentPicture();
	const char* getFileBackCardPicture();

private:
	int _cardsCount;
	int _order;
	int _position;
	int _posX;
	int _posY;
	int _cardPosX;
	int _cardPosY;
	int _defaultChangeCardPos;
	int _width;
	int _height;
	int _cardWidth;
	int _cardHeight;
	const char* _fileOpponentPicture;
	const char* _fileBackCardPicture;   
};

