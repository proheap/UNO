#include "WindowObject.h"

WindowObject::WindowObject(const char * texturesheet, SDL_Renderer * renderer, int x, int y, int w, int h) :
	_renderer(renderer),
	_objTexture(TextureManager::LoadTexture(texturesheet, renderer)),
	_posX(x),
	_posY(y),
	_width(w),
	_height(h)
{

}

WindowObject::~WindowObject()
{
	SDL_DestroyTexture(_objTexture);
}

void WindowObject::setTexture(const char * texturesheet, SDL_Renderer * renderer)
{
	_objTexture = TextureManager::LoadTexture(texturesheet, renderer);
}

void WindowObject::update()
{
	_destRect.x = _posX;
	_destRect.y = _posY;
	_destRect.w = _width;
	_destRect.h = _height;
}

void WindowObject::render()
{
	if (_objTexture) {
		SDL_RenderCopy(_renderer, _objTexture, nullptr, &_destRect);
	}
}

int WindowObject::getPosX()
{
	return _posX;
}

void WindowObject::setPosX(int posX)
{
	_posX = posX;
}

int WindowObject::getPosY()
{
	return _posY;
}

void WindowObject::setPosY(int posY)
{
	_posX = posY;
}

int WindowObject::getWidth()
{
	return _width;
}

int WindowObject::getHeight()
{
	return _height;
}

void WindowObject::hideTexture()
{
	_objTexture = nullptr;
}