#include "Table.h"

Table::Table(SDL_Renderer* renderer) :
	_renderer(renderer),
	_cardPosX(640 - (110 / 2)),
	_cardPosY(360 - (156 / 2)),
	_deckPosX(_cardPosX + 200),
	_deckPosY(_cardPosY)
{
	_deckObject = new WindowObject(FILE_DECK, renderer, _deckPosX, _deckPosY, 110, 156);
}

Table::~Table()
{
	for (Card* card : _playedCards) {
		delete card;
		card = nullptr;
	}
	
	delete _deckObject;
	_deckObject = nullptr;
}

void Table::setCard(Card* card)
{
	WindowObject* playedCardObj = new WindowObject(card->getFileCardPicture(), _renderer, _cardPosX, _cardPosY, card->getWidth(), card->getHeight());
	
	_playedCards.push_back(card);
	_playedCardsObjects.push_back(playedCardObj);
}

int Table::getCardPosX()
{
	return _cardPosX;
}

int Table::getCardPosY()
{
	return _cardPosY;
}


Card* Table::getPlayedCard()
{
	return _playedCards.back();
}

WindowObject * Table::getDeckObject()
{
	return _deckObject;
}

WindowObject * Table::getPlayedCardObject()
{
	return _playedCardsObjects.back();
}