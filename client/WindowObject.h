#pragma once

#include "heap_monitor.h"
#include "TextureManager.h"

class WindowObject
{
public:
	WindowObject(const char * texturesheet, SDL_Renderer * renderer, int x, int y, int w, int h);
	~WindowObject();
	void setTexture(const char * texturesheet, SDL_Renderer * renderer);
	void update();
	void render();
	int getPosX();
	void setPosX(int posX);
	int getPosY();
	void setPosY(int posY);
	int getWidth();
	int getHeight();
	void hideTexture();

private:
	int _posX;
	int _posY;
	int _width;
	int _height;
	SDL_Texture* _objTexture;
	SDL_Rect _destRect;
	SDL_Renderer* _renderer;
};