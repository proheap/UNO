#include "Player.h"

Player::Player(int order, SDL_Renderer* renderer):
	_order(order),
	_renderer(renderer),
	_cardPosX(CARD_POSX),
	_cardPosY(CARD_POSY)
{

}

Player::~Player()
{
	for (Card* card : _playerHand) {
		delete card;
		card = nullptr;
	}
	for (WindowObject* card : _playerHandObjects) {
		delete card;
		card = nullptr;
	}
}

void Player::addCard(char cardColor, char cardSymbol)
{
	Card* card = new Card(cardColor, cardSymbol, _cardPosX, _cardPosY);
	WindowObject* cardObj = new WindowObject(card->getFileCardPicture(), _renderer, _cardPosX, _cardPosY, card->getWidth(), card->getHeight());
	_playerHand.push_back(card);
	_playerHandObjects.push_back(cardObj);
	_cardPosX += card->getWidth();
}

void Player::playCard(int order)
{
	Card* playedCard = _playerHand.at(order);
	_playerHand.erase(_playerHand.begin() + order);
	delete playedCard;
}

void Player::updateCardPosX()
{
	_cardPosX -= CARD_WIDTH;
}

WindowObject* Player::showCard() {
	return _playerHandObjects.back();
}

vector<Card*> Player::getPlayerHand()
{
	return _playerHand;
}