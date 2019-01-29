#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define FILE_BACKGROUND "assets/background.png"
#define FILE_LOGO "assets/logo.png"
#define FILE_CREATE_GAME_BTN "assets/button_create-game_release.png"
#define FILE_CREATE_GAME_PUSH "assets/button_create-game_push.png"
#define FILE_JOIN_GAME_BTN "assets/button_join-game_release.png"
#define FILE_JOIN_GAME_PUSH "assets/button_join-game_push.png"
#define FILE_QUIT_GAME_BTN "assets/button_quit_release.png"
#define FILE_QUIT_GAME_PUSH "assets/button_quit_push.png"
#define FILE_MAIN_MENU_BTN "assets/button_main-menu_release.png"
#define FILE_MAIN_MENU_PUSH "assets/button_main-menu_push.png"
#define FILE_START_GAME_BTN "assets/button_start-game_release.png"
#define FILE_START_GAME_PUSH "assets/button_start-game_push.png"
#define FILE_CREATE_WAITING "assets/create-waiting.png"
#define FILE_JOIN_WAITING "assets/join-waiting.png"
#define FILE_GAME_OVER "assets/game-over.png"
#define FILE_WINNER "assets/winner.png"
#define BUTTON_WIDTH 200
#define BUTTON_HEIGHT 50

#pragma comment(lib,"ws2_32.lib")
#include <WinSock2.h>
#include <iostream>
#include <fstream>
#include <thread>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <fcntl.h>

#include "heap_monitor.h"
#include "WindowObject.h"
#include "WindowState.h"
#include "Client.h"
#include "Game.h"

using namespace std;

class Window
{
public:
	Window();
	~Window();

	void init(const char* title, int posX, int posY, int width, int height, bool fullscreen);
	void recvCycle();
	void pollEvents();
	void update();
	void render();
	void clean();
	void cycle();

	inline bool running() { return _open; };

private:
	//insert ip of server here
	char _ip[16] = "insert ip here";
	char _buffer[9];
	Client* _client;
	SOCKET _socket;
	std::thread _receiver;
	std::mutex _mutex;
	std::condition_variable _pauseCond;
	bool _pause;
	string _joinID;
	int _stateCycle;

	bool _open;
	SDL_Window* _window;
	SDL_Renderer* _renderer;
	vector<WindowObject*> _objectsMenu;
	vector<WindowObject*> _objectsCreateGame;
	vector<WindowObject*> _objectsJoinGame;
	vector<WindowObject*> _objectsGame;
	vector<WindowObject*> _objectsGameOver;
	vector<WindowObject*> _objectsWinner;
	WindowState::WinState _state;
	string _title;
	int _width;
	int _height;
	
	Game* _game;

	int _errConnect;
	bool _gameCreated;
	bool _startGame;
	bool _connected;
	int _playerOrder;
	bool _render;
	bool _playerTurn;
	bool _changeColor;
	bool _oneCard;
};

