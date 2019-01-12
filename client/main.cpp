#include <iostream>

#include "heap_monitor.h"
#include "Window.h"

using namespace std;

int main(int argc, char **argv)
{
	initHeapMonitor();

	Window *window;
	window = new Window();

	window->init("Hra UNO", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, false);

	window->cycle();

	window->clean();

	delete window;
	window = nullptr;

    return EXIT_SUCCESS;
};