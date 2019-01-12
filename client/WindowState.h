#pragma once

#include "heap_monitor.h"

class WindowState
{
public:
	enum WinState
	{
		MENU,
		CREATE_GAME,
		JOIN_GAME,
		START_GAME,
		GAME_OVER,
		WINNER
	};
};