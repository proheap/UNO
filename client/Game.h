#pragma once

#define FILE_CHANGE_COLOR "assets/change_color.png"
#define CHANGE_COLOR_POSX 400
#define CHANGE_COLOR_POSY 285
#define CHANGE_COLOR_WIDTH 150
#define FILE_BLUE "assets/blue.png"
#define FILE_RED "assets/red.png"
#define FILE_GREEN "assets/green.png"
#define FILE_YELLOW "assets/yellow.png"
#define COLOR_POSX 585
#define COLOR_POSY 415
#define COLOR_WIDTH 110
#define FILE_UNO_BUTTON "assets/uno-button.png"
#define UNO_BUTTON_POSX 1100
#define UNO_BUTTON_POSY 450
#define UNO_BUTTON_WIDTH 130
#define UNO_BUTTON_HEIGHT 100

#include <iostream>
#include <array>

#include "heap_monitor.h"
#include "Player.h"
#include "Opponent.h"
#include "Table.h"
#include "WindowObject.h"

using namespace std;

class Game
{
public:
    Game(SDL_Renderer* renderer);
    ~Game();
	void createPlayer(int order);
	void addOpponent(int order);
	void startGame();
	void showOpponentsCards();
	void showUnoButton();
	void hideUnoButton();
	void updateOpponentCards(int opponent, int cardsCount);
	void addPlayerCard(char cardColor, char cardSymbol);
	void addPlayedCard(char cardColor, char cardSymbol);
	void playRound(int orderCard);
	bool pollEventsGame(SDL_Event event, char* message);
	bool eventUnoButton(SDL_Event event, char* message, bool oneCard);
	vector<Opponent*>* getOpponents();
	vector<WindowObject*>* getGameObjects();
	vector<WindowObject*>* getPlayerHandObjects();
	vector<WindowObject*>* getOpponentsHandObjects();

private:
	vector<Opponent*> _opponents;
    Table* _table;
	Player* _player;
	int _playerOrder;
	bool _changingColor;
	bool _changedColor;
	bool _unoPressed;

	SDL_Renderer* _renderer;
	vector<WindowObject*> _gameObjects;
	vector<WindowObject*> _playerHandObjects;
	vector<WindowObject*> _opponentsHandObjects;
	WindowObject* _deckObject;
	WindowObject* _playedCardObject;
	WindowObject* _changeColorObject;
	WindowObject* _colorBlueObject;
	WindowObject* _colorRedObject;
	WindowObject* _colorGreenObject;
	WindowObject* _colorYellowObject;
	WindowObject* _unoButton;
};