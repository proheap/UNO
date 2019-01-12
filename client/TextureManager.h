#pragma once

#include <SDL.h>
#include <SDL_image.h>

#include "heap_monitor.h"

class TextureManager
{
public:
	static SDL_Texture* LoadTexture(const char* fileName, SDL_Renderer* renderer);
};

