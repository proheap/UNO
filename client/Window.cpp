#include "Window.h"

Window::Window() :
	_render(true),
	_state(WindowState::MENU),
	_buffer("00000000"),
	_connected(false),
	_errConnect(1),
	_stateCycle(1),
	_gameCreated(false),
	_pause(false),
	_playerTurn(false),
	_changeColor(false),
	_oneCard(false)
{
	_client = new Client(_ip, 5555);
	_socket = _client->getSocket();

	ifstream myfile("joinID.txt");
	if (myfile.is_open())
	{
		getline(myfile, _joinID);
		myfile.close();
	}
	_receiver = std::thread(&Window::recvCycle, this);
}

Window::~Window() 
{
	_objectsCreateGame.erase(_objectsCreateGame.begin());
	_objectsJoinGame.erase(_objectsJoinGame.begin());
	_objectsGame.erase(_objectsGame.begin());
	_objectsGameOver.erase(_objectsGameOver.begin());
	_objectsWinner.erase(_objectsWinner.begin());
	_objectsJoinGame.erase(_objectsJoinGame.begin());

	for (WindowObject* windowObject : _objectsMenu) {
		delete windowObject;
		windowObject = nullptr;
	}

	for (WindowObject* windowObject : _objectsCreateGame) {
		delete windowObject;
		windowObject = nullptr;
	}

	for (WindowObject* windowObject : _objectsJoinGame) {
		delete windowObject;
		windowObject = nullptr;
	}

	for (WindowObject* windowObject : _objectsGameOver) {
		delete windowObject;
		windowObject = nullptr;
	}

	for (WindowObject* windowObject : _objectsWinner) {
		delete windowObject;
		windowObject = nullptr;
	}
	if (_gameCreated) {
		delete _game;
		_game = nullptr;
	}

	_window = nullptr;

	_renderer = nullptr;

	delete _client;
	_client = nullptr;
}

void Window::init(const char * title, int posX, int posY, int width, int height, bool fullscreen)
{
	int flags = 0;
	if (fullscreen) {
		flags = SDL_WINDOW_FULLSCREEN;
	}
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
		_window = SDL_CreateWindow(title, posX, posY, width, height, flags);

		_renderer = SDL_CreateRenderer(_window, -1, 0);
		if (_renderer) {
			SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 0);
			cout << "Renderer created!" << endl;
		}

		_open = true;
	}
	else {
		cout << "Failed SDL initialize!" << endl;
		_open = false;
	}

	WindowObject* background = new WindowObject(FILE_BACKGROUND, _renderer, 0, 0, 1280, 720);
	_objectsMenu.push_back(background);
	_objectsCreateGame.push_back(background);
	_objectsJoinGame.push_back(background);
	_objectsGame.push_back(background);
	_objectsGameOver.push_back(background);
	_objectsWinner.push_back(background);
	WindowObject* logo = new WindowObject(FILE_LOGO, _renderer, 640 - (400 / 2), 10, 400, 281);
	_objectsMenu.push_back(logo);
	WindowObject* createGame = new WindowObject(FILE_CREATE_GAME_BTN, _renderer, 640 - (200 / 2), 350, BUTTON_WIDTH, BUTTON_HEIGHT);
	_objectsMenu.push_back(createGame);
	WindowObject* joinGame = new WindowObject(FILE_JOIN_GAME_BTN, _renderer, 640 - (200 / 2), 420, BUTTON_WIDTH, BUTTON_HEIGHT);
	_objectsMenu.push_back(joinGame);
	WindowObject* quitButton = new WindowObject(FILE_QUIT_GAME_BTN, _renderer, 640 - (200 / 2), 490, BUTTON_WIDTH, BUTTON_HEIGHT);
	_objectsMenu.push_back(quitButton);
	WindowObject* menuButton = new WindowObject(FILE_MAIN_MENU_BTN, _renderer, 20, 20, 200, 50);
	_objectsCreateGame.push_back(menuButton);
	_objectsJoinGame.push_back(menuButton);
	_objectsGame.push_back(menuButton);
	WindowObject* startButton = new WindowObject(FILE_START_GAME_BTN, _renderer, 640 - (200 / 2), 360 - (50 / 2), BUTTON_WIDTH, BUTTON_HEIGHT);
	_objectsCreateGame.push_back(startButton);
	_objectsCreateGame.at(2)->hideTexture();
	WindowObject* createWait = new WindowObject(FILE_CREATE_WAITING, _renderer, 640 - (200 / 2), 360 - (66 / 2), BUTTON_WIDTH, 66);
	_objectsCreateGame.push_back(createWait);
	WindowObject* joinWait = new WindowObject(FILE_JOIN_WAITING, _renderer, 640 - (200 / 2), 360 - (66 / 2), BUTTON_WIDTH, 66);
	_objectsJoinGame.push_back(joinWait);
	WindowObject* objectGameOver = new WindowObject(FILE_GAME_OVER, _renderer, 640 - (528 / 2), 360 - (528 / 2), 528, 528);
	_objectsGameOver.push_back(objectGameOver);
	WindowObject* menuGameOverButton = new WindowObject(FILE_MAIN_MENU_BTN, _renderer, 640 - (200 / 2), 540, BUTTON_WIDTH, BUTTON_HEIGHT);
	_objectsGameOver.push_back(menuGameOverButton);
	WindowObject* objectWinner = new WindowObject(FILE_WINNER, _renderer, 640 - (570 / 2), 360 - (340 / 2), 570, 340);
	_objectsWinner.push_back(objectWinner);
	WindowObject* menuWinnerButton = new WindowObject(FILE_MAIN_MENU_BTN, _renderer, 640 - (200 / 2), 540, BUTTON_WIDTH, BUTTON_HEIGHT);
	_objectsWinner.push_back(menuWinnerButton);
}

void Window::recvCycle()
{
	char buffer[9];
	buffer[8] = '\0';
	int valRead;
	while (_stateCycle) {
		valRead = recv(_socket, buffer, 8, 0);

		switch (buffer[0])
		{
		case '1':
			switch (buffer[1])
			{
			case '1':
				_errConnect = buffer[2] - '0';
				break;
			case '2':
				_errConnect = buffer[2] - '0';
				_playerOrder = buffer[3] - '0';
				break;
			default:
				break;
			}
			break;
		case '2':
			switch (buffer[1])
			{
			case '1':
				_game->addOpponent(buffer[2] - '0');
				break;
			case '2':
				_render = false;
				_startGame = true;
				break;
			default:
				break;
			}
			break;
		case '3':
			switch (buffer[1])
			{
			case '1':
				_render = false;
				Sleep(300);
				_game->addPlayerCard(buffer[2], buffer[3]);
				_render = true;
				break;
			case '2':
				_render = false;
				Sleep(300);
				_game->addPlayedCard(buffer[2], buffer[3]);
				_render = true;
				break;
			case '3':
				_render = false;
				if (_game->getPlayerHandObjects()->size() == 2) {
					Sleep(200);
					_game->showUnoButton();
				}
				_render = true;
				_buffer[4] = '0';
				_playerTurn = true;
				break;
			case '6':
				_render = false;
				Sleep(200);
				for (int i = 0; i < static_cast<int>(_game->getOpponents()->size()); i++) {
					_game->updateOpponentCards(i, buffer[i+2] - '0');
				}
				_game->showOpponentsCards();
				_render = true;
				break;
			case '7':
				_render = false;
				Sleep(200);
				_game->showUnoButton();
				_oneCard = true; 
				_render = true;
				break;
			case '8':
				_render = false;
				_game->hideUnoButton();
				_oneCard = false;
				_playerTurn = false;
				_render = true;
				break;
			case '9':
				_render = false;
				_game->hideUnoButton();
				_oneCard = false;
				_render = true;
			default:
				break;
			}
			break;
		case '4':
			switch (buffer[1])
			{
			case '1':
				_state = WindowState::WINNER;
				break;
			case '2':
				_state = WindowState::GAME_OVER;
				break;
			case '3':
				_state = WindowState::MENU;
				break;
			case '4':
				_stateCycle = 0;
				break;
			default:
				break;
			}
		default:
			break;
		}

		std::cout << buffer << endl;
	}
	std::cout << "Finish!" << endl;
}

void Window::pollEvents()
{
	SDL_Event event;
	SDL_PollEvent(&event);

	switch (event.type)
	{
	case SDL_QUIT:
		_buffer[0] = '4';
		_buffer[1] = '3';
		send(_socket, _buffer, 8, 0);
		_receiver.join();
		_open = false;
		break;
	case SDL_KEYDOWN:
		switch (event.key.keysym.sym) {
		case SDLK_ESCAPE:
			_buffer[0] = '4';
			_buffer[1] = '3';
			send(_socket, _buffer, 8, 0);
			_receiver.join();
			_open = false;
			break;
		}
	default:
		switch (_state)
		{
		case WindowState::MENU:
			switch (event.type) {
			case SDL_MOUSEMOTION:
				if ((event.button.x > _objectsMenu.at(2)->getPosX() && event.button.x < _objectsMenu.at(2)->getPosX() + _objectsMenu.at(2)->getWidth()) && (event.button.y > _objectsMenu.at(2)->getPosY() && event.button.y < _objectsMenu.at(2)->getPosY() + _objectsMenu.at(2)->getHeight())) {
					_objectsMenu.at(2)->setTexture(FILE_CREATE_GAME_PUSH, _renderer);
				}
				else {
					_objectsMenu.at(2)->setTexture(FILE_CREATE_GAME_BTN, _renderer);
				}
				if ((event.button.x > _objectsMenu.at(3)->getPosX() && event.button.x < _objectsMenu.at(3)->getPosX() + _objectsMenu.at(3)->getWidth()) && (event.button.y > _objectsMenu.at(3)->getPosY() && event.button.y < _objectsMenu.at(3)->getPosY() + _objectsMenu.at(3)->getHeight())) {
					_objectsMenu.at(3)->setTexture(FILE_JOIN_GAME_PUSH, _renderer);
				}
				else {
					_objectsMenu.at(3)->setTexture(FILE_JOIN_GAME_BTN, _renderer);
				}
				if ((event.button.x > _objectsMenu.at(4)->getPosX() && event.button.x < _objectsMenu.at(4)->getPosX() + _objectsMenu.at(4)->getWidth()) && (event.button.y > _objectsMenu.at(4)->getPosY() && event.button.y < _objectsMenu.at(4)->getPosY() + _objectsMenu.at(4)->getHeight())) {
					_objectsMenu.at(4)->setTexture(FILE_QUIT_GAME_PUSH, _renderer);
				}
				else {
					_objectsMenu.at(4)->setTexture(FILE_QUIT_GAME_BTN, _renderer);
				}
				break;
			case SDL_MOUSEBUTTONDOWN:
				if ((event.button.x > _objectsMenu.at(2)->getPosX() && event.button.x < _objectsMenu.at(2)->getPosX() + _objectsMenu.at(2)->getWidth()) && (event.button.y > _objectsMenu.at(2)->getPosY() && event.button.y < _objectsMenu.at(2)->getPosY() + _objectsMenu.at(2)->getHeight())) {
					_buffer[0] = '1';
					_buffer[1] = '1';
					_buffer[2] = _joinID[0];
					_buffer[3] = _joinID[1];
					send(_socket, _buffer, 8, 0);
					Sleep(500);
					if (_errConnect == 1) {
						MessageBoxA(NULL, "Room already exists!", "Creating room failed!", MB_OK | MB_ICONERROR);
					}
					else {
						_state = WindowState::CREATE_GAME;
						_game = new Game(_renderer);
						_gameCreated = true;
						_game->createPlayer(1);
					}
				}
				if ((event.button.x > _objectsMenu.at(3)->getPosX() && event.button.x < _objectsMenu.at(3)->getPosX() + _objectsMenu.at(3)->getWidth()) && (event.button.y > _objectsMenu.at(3)->getPosY() && event.button.y < _objectsMenu.at(3)->getPosY() + _objectsMenu.at(3)->getHeight())) {
					_buffer[0] = '1';
					_buffer[1] = '2';
					_buffer[2] = _joinID[0];
					_buffer[3] = _joinID[1];
					send(_socket, _buffer, 8, 0);
					Sleep(500);
					if (_errConnect == 1) {
						MessageBoxA(NULL, "Room with ID not exists!", "Joining game failed!", MB_OK | MB_ICONERROR);
					}
					else {
						_state = WindowState::JOIN_GAME;
						_render = false;
						_game = new Game(_renderer);
						_gameCreated = true;
						_game->createPlayer(_playerOrder);
						for (int i = 1; i < _playerOrder; i++) {
							_game->addOpponent(i);
						}
						_render = true;
					}
					
				}
				if ((event.button.x > _objectsMenu.at(4)->getPosX() && event.button.x < _objectsMenu.at(4)->getPosX() + _objectsMenu.at(4)->getWidth()) && (event.button.y > _objectsMenu.at(4)->getPosY() && event.button.y < _objectsMenu.at(4)->getPosY() + _objectsMenu.at(4)->getHeight())) {		
					_buffer[0] = '4';
					_buffer[1] = '3';
					send(_socket, _buffer, 8, 0);
					_receiver.join();
					_open = false;
				}
				break;
			default:
				break;
			}
			break;
		case WindowState::CREATE_GAME:
			if (_game->getOpponents()->size() >= 1 && _connected == false) {
				_objectsCreateGame.at(3)->hideTexture();
				_objectsCreateGame.at(2)->setTexture(FILE_START_GAME_BTN, _renderer);
				_connected = true;
			
			}
			switch (event.type) {
			case SDL_MOUSEMOTION:
				if ((event.button.x > _objectsCreateGame.at(1)->getPosX() && event.button.x < _objectsCreateGame.at(1)->getPosX() + _objectsCreateGame.at(1)->getWidth()) && (event.button.y > _objectsCreateGame.at(1)->getPosY() && event.button.y < _objectsCreateGame.at(1)->getPosY() + _objectsCreateGame.at(1)->getHeight())) {
					_objectsCreateGame.at(1)->setTexture(FILE_MAIN_MENU_PUSH, _renderer);
				}
				else {
					_objectsCreateGame.at(1)->setTexture(FILE_MAIN_MENU_BTN, _renderer);
				}
				if (_game->getOpponents()->size() >= 1) {
					if ((event.button.x > _objectsCreateGame.at(2)->getPosX() && event.button.x < _objectsCreateGame.at(2)->getPosX() + _objectsCreateGame.at(2)->getWidth()) && (event.button.y > _objectsCreateGame.at(2)->getPosY() && event.button.y < _objectsCreateGame.at(2)->getPosY() + _objectsCreateGame.at(2)->getHeight())) {
						_objectsCreateGame.at(2)->setTexture(FILE_START_GAME_PUSH, _renderer);
					}
					else {
						_objectsCreateGame.at(2)->setTexture(FILE_START_GAME_BTN, _renderer);
					}
				}
				break;
			case SDL_MOUSEBUTTONDOWN:
				if ((event.button.x > _objectsGame.at(1)->getPosX() && event.button.x < _objectsGame.at(1)->getPosX() + _objectsGame.at(1)->getWidth()) && (event.button.y > _objectsGame.at(1)->getPosY() && event.button.y < _objectsGame.at(1)->getPosY() + _objectsGame.at(1)->getHeight())) {
					_errConnect = 1;
					_state = WindowState::MENU;
				}
				if (_game->getOpponents()->size() >= 1) {
					if ((event.button.x > _objectsCreateGame.at(2)->getPosX() && event.button.x < _objectsCreateGame.at(2)->getPosX() + _objectsCreateGame.at(2)->getWidth()) && (event.button.y > _objectsCreateGame.at(2)->getPosY() && event.button.y < _objectsCreateGame.at(2)->getPosY() + _objectsCreateGame.at(2)->getHeight())) {
						_buffer[0] = '2';
						_buffer[1] = '2';
						send(_socket, _buffer, 8, 0);

						_render = false;
						_game->startGame();
						_state = WindowState::START_GAME;
					}
				}
				break;
			}
			break;
		case WindowState::JOIN_GAME:
			if (_startGame == true) {
				_game->startGame();
				_state = WindowState::START_GAME;
			}
			switch (event.type) {
			case SDL_MOUSEMOTION:
				if ((event.button.x > _objectsJoinGame.at(1)->getPosX() && event.button.x < _objectsJoinGame.at(1)->getPosX() + _objectsJoinGame.at(1)->getWidth()) && (event.button.y > _objectsJoinGame.at(1)->getPosY() && event.button.y < _objectsJoinGame.at(1)->getPosY() + _objectsJoinGame.at(1)->getHeight())) {
					_objectsJoinGame.at(1)->setTexture(FILE_MAIN_MENU_PUSH, _renderer);
				}
				else {
					_objectsJoinGame.at(1)->setTexture(FILE_MAIN_MENU_BTN, _renderer);
				}
				break;
			case SDL_MOUSEBUTTONDOWN:
				if ((event.button.x > _objectsJoinGame.at(1)->getPosX() && event.button.x < _objectsJoinGame.at(1)->getPosX() + _objectsJoinGame.at(1)->getWidth()) && (event.button.y > _objectsJoinGame.at(1)->getPosY() && event.button.y < _objectsJoinGame.at(1)->getPosY() + _objectsJoinGame.at(1)->getHeight())) {
					_errConnect = 1;
					_state = WindowState::MENU;
					_render = false;
				}
				break;
			}
			break;
		case WindowState::START_GAME:
			switch (event.type) {
			case SDL_MOUSEMOTION:
				if ((event.button.x > _objectsGame.at(1)->getPosX() && event.button.x < _objectsGame.at(1)->getPosX() + _objectsGame.at(1)->getWidth()) && (event.button.y > _objectsGame.at(1)->getPosY() && event.button.y < _objectsGame.at(1)->getPosY() + _objectsGame.at(1)->getHeight())) {
					_objectsGame.at(1)->setTexture(FILE_MAIN_MENU_PUSH, _renderer);
				}
				else {
					_objectsGame.at(1)->setTexture(FILE_MAIN_MENU_BTN, _renderer);
				}
				break;
			case SDL_MOUSEBUTTONDOWN:
				if ((event.button.x > _objectsGame.at(1)->getPosX() && event.button.x < _objectsGame.at(1)->getPosX() + _objectsGame.at(1)->getWidth()) && (event.button.y > _objectsGame.at(1)->getPosY() && event.button.y < _objectsGame.at(1)->getPosY() + _objectsGame.at(1)->getHeight())) {
					_errConnect = 1;
					_state = WindowState::MENU;
				}
				break;
			}
			if (_playerTurn) {
				_game->eventUnoButton(event, _buffer, _oneCard);
				if (_game->pollEventsGame(event, _buffer)) {
					send(_socket, _buffer, 8, 0);
					_playerTurn = false;
				}
			}
			if (_oneCard) {
				if (_game->eventUnoButton(event, _buffer, _oneCard)) {
					send(_socket, _buffer, 8, 0);
					_oneCard = false;
				}
			}
			break;
		case WindowState::GAME_OVER:
			switch (event.type) {
			case SDL_MOUSEMOTION:
				if ((event.button.x > _objectsGameOver.at(2)->getPosX() && event.button.x < _objectsGameOver.at(2)->getPosX() + _objectsGameOver.at(2)->getWidth()) && (event.button.y > _objectsGameOver.at(2)->getPosY() && event.button.y < _objectsGameOver.at(2)->getPosY() + _objectsGameOver.at(2)->getHeight())) {
					_objectsGameOver.at(2)->setTexture(FILE_MAIN_MENU_PUSH, _renderer);
				}
				else {
					_objectsGameOver.at(2)->setTexture(FILE_MAIN_MENU_BTN, _renderer);
				}
				break;
			case SDL_MOUSEBUTTONDOWN:
				if ((event.button.x > _objectsGameOver.at(2)->getPosX() && event.button.x < _objectsGameOver.at(2)->getPosX() + _objectsGameOver.at(2)->getWidth()) && (event.button.y > _objectsGameOver.at(2)->getPosY() && event.button.y < _objectsGameOver.at(2)->getPosY() + _objectsGameOver.at(2)->getHeight())) {
					_errConnect = 1;
					_state = WindowState::MENU;
				}
			}
			break;
		case WindowState::WINNER:
			switch (event.type) {
			case SDL_MOUSEMOTION:
				if ((event.button.x > _objectsWinner.at(2)->getPosX() && event.button.x < _objectsWinner.at(2)->getPosX() + _objectsWinner.at(2)->getWidth()) && (event.button.y > _objectsWinner.at(2)->getPosY() && event.button.y < _objectsWinner.at(2)->getPosY() + _objectsWinner.at(2)->getHeight())) {
					_objectsWinner.at(2)->setTexture(FILE_MAIN_MENU_PUSH, _renderer);
				}
				else {
					_objectsWinner.at(2)->setTexture(FILE_MAIN_MENU_BTN, _renderer);
				}
				break;
			case SDL_MOUSEBUTTONDOWN:
				if ((event.button.x > _objectsWinner.at(2)->getPosX() && event.button.x < _objectsWinner.at(2)->getPosX() + _objectsWinner.at(2)->getWidth()) && (event.button.y > _objectsWinner.at(2)->getPosY() && event.button.y < _objectsWinner.at(2)->getPosY() + _objectsWinner.at(2)->getHeight())) {
					_errConnect = 1;
					_state = WindowState::MENU;
				}
			}
			break;
		default:
			break;
		}
		break;
	}
}

void Window::update() 
{
	switch (_state)
	{
	case WindowState::MENU:
		for (WindowObject* windowObject : _objectsMenu)
		{
			windowObject->update();
		}
		break;
	case WindowState::CREATE_GAME:
		for (WindowObject* windowObject : _objectsCreateGame)
		{
			windowObject->update();
		}
		break;
	case WindowState::JOIN_GAME:
		for (WindowObject* windowObject : _objectsJoinGame)
		{
			windowObject->update();
		}
		break;
	case WindowState::START_GAME:
		for (WindowObject* windowObject : _objectsGame)
		{
			windowObject->update();
		}
		for (WindowObject* windowObject : *_game->getGameObjects())
		{
			windowObject->update();
		}
		for (WindowObject* windowObject : *_game->getPlayerHandObjects())
		{
			windowObject->update();
		}
		for (WindowObject* windowObject : *_game->getOpponentsHandObjects())
		{
			windowObject->update();
		}
		break;
	case WindowState::GAME_OVER:
		for (WindowObject* windowObject : _objectsGameOver)
		{
			windowObject->update();
		}
		break;
	case WindowState::WINNER:
		for (WindowObject* windowObject : _objectsWinner)
		{
			windowObject->update();
		}
		break;
	default:
		break;
	}
}

void Window::render()
{
	SDL_RenderClear(_renderer);

	switch (_state)
	{
	case WindowState::MENU:
		for (WindowObject* windowObject : _objectsMenu)
		{
			windowObject->render();
		}		
		break;
	case WindowState::CREATE_GAME:
		for (WindowObject* windowObject : _objectsCreateGame)
		{
			windowObject->render();
		}
		break;
	case WindowState::JOIN_GAME:
		for (WindowObject* windowObject : _objectsJoinGame)
		{
			windowObject->render();
		}
		break;
	case WindowState::START_GAME:
		for (WindowObject* windowObject : _objectsGame)
		{
			windowObject->render();
		}
		for (WindowObject* windowObject : *_game->getGameObjects())
		{
			windowObject->render();
		}
		for (WindowObject* windowObject : *_game->getPlayerHandObjects())
		{
			windowObject->render();
		}
		for (WindowObject* windowObject : *_game->getOpponentsHandObjects())
		{
			windowObject->render();
		}
		break;
	case WindowState::GAME_OVER:
		for (WindowObject* windowObject : _objectsGameOver)
		{
			windowObject->render();
		}
		break;
	case WindowState::WINNER:
		for (WindowObject* windowObject : _objectsWinner)
		{
			windowObject->render();
		}
		break;
	default:
		break;
	}

	SDL_RenderPresent(_renderer);
}

void Window::clean() 
{
	SDL_DestroyWindow(_window);
	SDL_DestroyRenderer(_renderer);
	SDL_Quit();

	cout << "Game ended!" << endl;
}

void Window::cycle()
{
	while (_open) 
	{
		while (_render) {
			pollEvents();
			update();
			render();
			if (!_open) {
				break;
			}
		}
	}
}