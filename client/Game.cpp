#include "Game.h"

Game::Game(SDL_Renderer* renderer) :
	_renderer(renderer),
	_table(new Table(renderer)),
	_player(nullptr),
	_changingColor(false),
	_changedColor(false),
	_unoPressed(false)
{
	_deckObject = _table->getDeckObject();
	_gameObjects.push_back(_deckObject);

	_changeColorObject = new WindowObject(FILE_CHANGE_COLOR, _renderer, CHANGE_COLOR_POSX, CHANGE_COLOR_POSY, CHANGE_COLOR_WIDTH, CHANGE_COLOR_WIDTH);
	_gameObjects.push_back(_changeColorObject);
	_changeColorObject->hideTexture();

	_colorBlueObject = new WindowObject(FILE_BLUE, _renderer, COLOR_POSX, COLOR_POSY, COLOR_WIDTH, COLOR_WIDTH);
	_gameObjects.push_back(_colorBlueObject);
	_colorBlueObject->hideTexture();
	_colorRedObject = new WindowObject(FILE_RED, _renderer, COLOR_POSX, COLOR_POSY, COLOR_WIDTH, COLOR_WIDTH);
	_gameObjects.push_back(_colorRedObject);
	_colorRedObject->hideTexture();
	_colorGreenObject = new WindowObject(FILE_GREEN, _renderer, COLOR_POSX, COLOR_POSY, COLOR_WIDTH, COLOR_WIDTH);
	_gameObjects.push_back(_colorGreenObject);
	_colorGreenObject->hideTexture();
	_colorYellowObject = new WindowObject(FILE_YELLOW, _renderer, COLOR_POSX, COLOR_POSY, COLOR_WIDTH, COLOR_WIDTH);
	_gameObjects.push_back(_colorYellowObject);
	_colorYellowObject->hideTexture();

	_unoButton = new WindowObject(FILE_UNO_BUTTON, _renderer, UNO_BUTTON_POSX, UNO_BUTTON_POSY, UNO_BUTTON_WIDTH, UNO_BUTTON_HEIGHT);
	_gameObjects.push_back(_unoButton);
	_unoButton->hideTexture();
}

Game::~Game()
{   
	_gameObjects.erase(_gameObjects.begin());

	for (Opponent* opponent : _opponents) {
		delete opponent;
		opponent = nullptr;
	}

	for (WindowObject* windowObject : _gameObjects) {
		delete windowObject;
		windowObject = nullptr;
	}

	for (WindowObject* opponentHandObject : _opponentsHandObjects) {
		delete opponentHandObject;
		opponentHandObject = nullptr;
	}

	delete _player;
	_player = nullptr;

    delete _table;
	_table = nullptr;
}

void Game::createPlayer(int order)
{
	_player = new Player(order, _renderer);
	_playerOrder = order;
	cout << "Hrac bol vytvoreny! V poradi: " << order << endl;
}

void Game::addOpponent(int order) 
{
	_opponents.push_back(new Opponent(order, _playerOrder));
	cout << "Super bol vytvoreny! V poradi: " << order << endl;
}

void Game::startGame() 
{
	for (Opponent* opponent : _opponents) {
		WindowObject* opponentObject = new WindowObject(opponent->getFileOpponentPicture(), _renderer, opponent->getPosX(), opponent->getPosY(), opponent->getWidth(), opponent->getHeight());
		_gameObjects.push_back(opponentObject);
	}

	showOpponentsCards();
}

void Game::showOpponentsCards()
{
	for (WindowObject* opponentHandObject : _opponentsHandObjects) {
		opponentHandObject->hideTexture();
		_gameObjects.push_back(opponentHandObject);
	}

	_opponentsHandObjects.clear();

	for (Opponent* opponent : _opponents) {
		opponent->setDefaultCardPos();
		for (int i = 0; i < opponent->getCardsCount(); i++)
		{
			WindowObject* opponentCardObj = new WindowObject(opponent->getFileBackCardPicture(), _renderer, opponent->getCardPosX(), opponent->getCardPosY(), opponent->getCardWidth(), opponent->getCardHeight());
			opponent->setCardPos(opponent->getChangeCardPos() + 20);
			_opponentsHandObjects.push_back(opponentCardObj);
		}
	}
}

void Game::showUnoButton()
{
	_unoButton->setTexture(FILE_UNO_BUTTON, _renderer);
}

void Game::hideUnoButton()
{
	_unoButton->hideTexture();
}

void Game::updateOpponentCards(int opponent, int cardsCount) 
{
	_opponents.at(opponent)->setCardsCount(cardsCount);
}

void Game::addPlayerCard(char cardColor, char cardSymbol)
{
	_player->addCard(cardColor, cardSymbol);
	_playerHandObjects.push_back(_player->showCard());
	if (static_cast<int>(_playerHandObjects.size()) > 2) {
		hideUnoButton();
	}
}

void Game::addPlayedCard(char cardColor, char cardSymbol)
{
	Card* playedCard = new Card(cardColor, cardSymbol, _table->getCardPosX(), _table->getCardPosY());
	_table->setCard(playedCard);

	if (_changingColor == false) {
		_changeColorObject->hideTexture();
	}

	if (_changedColor == false) {
		_colorBlueObject->hideTexture();
		_colorRedObject->hideTexture();
		_colorGreenObject->hideTexture();
		_colorYellowObject->hideTexture();
	}

	if (playedCard->getCardSymbol() == TypesOfCards::CHANGE_COLOR || playedCard->getCardSymbol() == TypesOfCards::PLUS_4)
	{
		switch (playedCard->getCardColor())
		{
		case TypesOfCards::BLUE:
			_colorBlueObject->setTexture(FILE_BLUE, _renderer);
			break;
		case TypesOfCards::RED:
			_colorRedObject->setTexture(FILE_RED, _renderer);
			break;
		case TypesOfCards::GREEN:
			_colorGreenObject->setTexture(FILE_GREEN, _renderer);
			break;
		default:
			_colorYellowObject->setTexture(FILE_YELLOW, _renderer);
			break;
		}
		_changedColor = false;
	}
	WindowObject* _playedCardObject = _table->getPlayedCardObject();
	_gameObjects.push_back(_playedCardObject);
	cout << "Karta vytvorena: " << cardColor << ", " << cardSymbol << endl;
}

void Game::playRound(int orderCard)
{
	_player->playCard(orderCard);
	for (int i = orderCard + 1; i < static_cast<int>(_playerHandObjects.size()); i++) {
		_playerHandObjects.at(i)->setPosX(_playerHandObjects.at(i)->getPosX() - CARD_WIDTH);
	}
	_player->updateCardPosX();
	_playerHandObjects.erase(_playerHandObjects.begin() + orderCard);
}

bool Game::pollEventsGame(SDL_Event event, char* message)
{
	if (!_changingColor) {
		switch (event.type) {
		case SDL_MOUSEBUTTONDOWN:
			for (int i = 0; i < static_cast<int>(_playerHandObjects.size()); i++) {
				if ((event.button.x > _playerHandObjects.at(i)->getPosX() && event.button.x < _playerHandObjects.at(i)->getPosX() + _playerHandObjects.at(i)->getWidth()) && (event.button.y > _playerHandObjects.at(i)->getPosY() && event.button.y < _playerHandObjects.at(i)->getPosY() + _playerHandObjects.at(i)->getHeight())) {
					if (_player->getPlayerHand().at(i)->getCardColor() == TypesOfCards::SPECIAL){	
						_changeColorObject->setTexture(FILE_CHANGE_COLOR, _renderer);
						_changingColor = true;
						message[0] = '3';
						message[1] = '4';
						message[2] = i + '0';
						playRound(i);
					}
					else if (_player->getPlayerHand().at(i)->getCardSymbol() == _table->getPlayedCard()->getCardSymbol()) {
						message[0] = '3';
						message[1] = '4';
						message[2] = i + '0';
						playRound(i);
						cout << "Sending!" << endl;
						return true;
					}
					else if (_player->getPlayerHand().at(i)->getCardColor() == _table->getPlayedCard()->getCardColor()) {
						message[0] = '3';
						message[1] = '4';
						message[2] = i + '0';
						playRound(i);
						cout << "Sending!" << endl;
						return true;
					}
				}
			}
			if ((event.button.x > _deckObject->getPosX() && event.button.x < _deckObject->getPosX() + _deckObject->getWidth()) && (event.button.y > _deckObject->getPosY() && event.button.y < _deckObject->getPosY() + _deckObject->getHeight())) {
				message[0] = '3';
				message[1] = '5';
				cout << "Sending!" << endl;
				return true;
			}
			break;
		default:
			break;
		}
	}
	else {
		switch (event.type) {
		case SDL_MOUSEBUTTONDOWN:
			if ((event.button.x > CHANGE_COLOR_POSX && event.button.x < CHANGE_COLOR_POSX + (CHANGE_COLOR_WIDTH / 2)) && (event.button.y > CHANGE_COLOR_POSY && event.button.y < CHANGE_COLOR_POSY + (CHANGE_COLOR_WIDTH / 2))) {
				message[3] = 'r';
				_changeColorObject->hideTexture();
				_changingColor = false;
				return true;
			}
			else if ((event.button.x > CHANGE_COLOR_POSX + (CHANGE_COLOR_WIDTH / 2) && event.button.x < (CHANGE_COLOR_POSX + CHANGE_COLOR_WIDTH)) && (event.button.y > CHANGE_COLOR_POSY && event.button.y < CHANGE_COLOR_POSY + (CHANGE_COLOR_WIDTH / 2))) {
				message[3] = 'g';
				_changeColorObject->hideTexture();
				_changingColor = false;
				return true;
			}
			else if ((event.button.x > CHANGE_COLOR_POSX && event.button.x < CHANGE_COLOR_POSX + (CHANGE_COLOR_WIDTH / 2)) && (event.button.y > CHANGE_COLOR_POSY + (CHANGE_COLOR_WIDTH / 2) && event.button.y < CHANGE_COLOR_POSY + CHANGE_COLOR_WIDTH)) {
				message[3] = 'b';
				_changeColorObject->hideTexture();
				_changingColor = false;
				return true;
			}
			else if ((event.button.x > CHANGE_COLOR_POSX + (CHANGE_COLOR_WIDTH / 2) && event.button.x < CHANGE_COLOR_POSX + CHANGE_COLOR_WIDTH ) && (event.button.y > CHANGE_COLOR_POSY + (CHANGE_COLOR_WIDTH / 2) && event.button.y < CHANGE_COLOR_POSY + CHANGE_COLOR_WIDTH)) {
				message[3] = 'y';
				_changeColorObject->hideTexture();
				_changingColor = false;
				return true;
			}
			break;
		default:
			break;
		}
	}
	return false;
}

bool Game::eventUnoButton(SDL_Event event, char* message, bool oneCard)
{
	if (!oneCard) {
		switch (event.type) {
		case SDL_MOUSEBUTTONDOWN:
			if ((event.button.x > UNO_BUTTON_POSX && event.button.x < UNO_BUTTON_POSX + UNO_BUTTON_WIDTH) && (event.button.y > UNO_BUTTON_POSY && event.button.y < UNO_BUTTON_POSX + UNO_BUTTON_HEIGHT)) {
				message[4] = '1';
				_unoButton->hideTexture();
				return true;
			}
			break;
		default:
			break;
		}
	} 
	else {
		switch (event.type) {
		case SDL_MOUSEBUTTONDOWN:
			if ((event.button.x > UNO_BUTTON_POSX && event.button.x < UNO_BUTTON_POSX + UNO_BUTTON_WIDTH) && (event.button.y > UNO_BUTTON_POSY && event.button.y < UNO_BUTTON_POSX + UNO_BUTTON_HEIGHT)) {
				message[0] = '3';
				message[1] = '7';
				_unoButton->hideTexture();
				return true;
			}
			break;
		default:
			break;
		}
	}
	return false;
}

vector<Opponent*>* Game::getOpponents()
{
	return &_opponents;
}

vector<WindowObject*>* Game::getGameObjects()
{
	return &_gameObjects;
}

vector<WindowObject*>* Game::getPlayerHandObjects()
{
	return &_playerHandObjects;
}

vector<WindowObject*>* Game::getOpponentsHandObjects()
{
	return &_opponentsHandObjects;
}
