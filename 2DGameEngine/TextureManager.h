#pragma once
#ifndef TextureManager_h
#define TextureManager_h

#include "Game.h"

class TextureManager {
public:
	static SDL_Texture* LoadTexture(const char* texture);
	static SDL_Texture* LoadFont(std::string fontPath, int fontSize, const char* fontText);
	static void Draw(SDL_Texture* tex, SDL_Rect src, SDL_Rect dest, SDL_RendererFlip flip, int angle);

};
#endif