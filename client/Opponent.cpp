#include "Opponent.h"

Opponent::Opponent(int order, int playerOrder):
	_cardsCount(7),
	_order(order),
	_position(0),
	_fileOpponentPicture(FILE_OPPONENT),
	_width(OPPONENT_WIDTH),
	_height(OPPONENT_HEIGHT),
	_cardWidth(0),
	_cardHeight(0),
	_defaultChangeCardPos(0)
{
	switch (playerOrder)
	{
	case 1:
		switch (order)
		{
		case 2:
			_position = 0;
			break;
		case 3:
			_position = 1;
			break;
		default:
			_position = 2;
			break;
		}
		break;
	case 2:
		switch (order)
		{
		case 1:
			_position = 2;
			break;
		case 3:
			_position = 0;
			break;
		default:
			_position = 1;
			break;
		}
		break;
	case 3:
		switch (order)
		{
		case 1:
			_position = 1;
			break;
		case 2:
			_position = 2;
			break;
		default:
			_position = 0;
			break;
		}
		break;
	default:
		switch (order)
		{
		case 1:
			_position = 2;
			break;
		case 2:
			_position = 1;
			break;
		default:
			_position = 0;
			break;
		}
		break;
	}
	setFileOpponentCard();
	setPosition();
}

Opponent::~Opponent()
{
	_fileOpponentPicture = nullptr;
	_fileBackCardPicture = nullptr;
}

void Opponent::setPosition()
{
	switch (_position)
	{
	case 0:
		_posX = 0;
		_posY = 360 - (_height / 2);
		_cardPosX = _posX + 100;
		_cardPosY = 100;
		_defaultChangeCardPos = _cardPosY;
		break;
	case 1:
		_posX = 640 - (_width / 2);
		_posY = 0;
		_cardPosX = 450;
		_cardPosY = _posY + 100;
		_defaultChangeCardPos = _cardPosX;
		break;
	default:
		_posX = 1280 - _width;
		_posY = 360 - (_height / 2);
		_cardPosX = _posX - getCardWidth();
		_cardPosY = 100;
		_defaultChangeCardPos = _cardPosY;
		break;
	}
}

void Opponent::setFileOpponentCard()
{
	switch (_position)
	{
	case 1:
		_fileBackCardPicture = FILE_BACK_CARD;
		_cardWidth = BACK_CARD_WIDTH;
		_cardHeight = BACK_CARD_HEIGHT;
		break;
	case 0:
	default:
		_fileBackCardPicture = FILE_BACKSIDE_CARD;
		_cardWidth = SIDE_CARD_WIDTH;
		_cardHeight = SIDE_CARD_HEIGHT;
		break;
	}
}

void Opponent::setCardsCount(int count)
{
	_cardsCount = count;
}

int Opponent::getCardsCount()
{
	return _cardsCount;
}

int Opponent::getOrder()
{
	return _order;
}

const char * Opponent::getFileOpponentPicture()
{
	return _fileOpponentPicture;
}

const char * Opponent::getFileBackCardPicture()
{
	return _fileBackCardPicture;
}

int Opponent::getPosX()
{
	return _posX;
}

int Opponent::getPosY()
{
	return _posY;
}

int Opponent::getChangeCardPos()
{
	switch (_position)
	{
	case 1:
		return _cardPosX;
		break;
	case 0:
	default:
		return _cardPosY;
	}
}

void Opponent::setCardPos(int cardPos)
{
	switch (_position)
	{
	case 1:
		_cardPosX = cardPos;
		break;
	case 0:
	default:
		_cardPosY = cardPos;
	}
}

void Opponent::setDefaultCardPos()
{
	setCardPos(_defaultChangeCardPos);
}

int Opponent::getCardPosX()
{
	return _cardPosX;
}

int Opponent::getCardPosY()
{
	return _cardPosY;
}

int Opponent::getWidth()
{
	return _width;
}

int Opponent::getCardWidth()
{
	return _cardWidth;
}

int Opponent::getHeight()
{
	return _height;
}

int Opponent::getCardHeight()
{
	return _cardHeight;
}
