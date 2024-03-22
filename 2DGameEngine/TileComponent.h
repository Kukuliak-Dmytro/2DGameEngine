#pragma once

#include "Components.h"

class TileComponent :public Component {
public:

	SDL_Texture* texture;
	SDL_Rect srcRect, destRect;
	Vector2D position;


	TileComponent() = default;

	~TileComponent()
	{
		SDL_DestroyTexture(texture);
	}

	TileComponent(int srcX, int srcY, int xpos, int ypos, const char* path)
	{
		texture = LoadTexture(path);

		position.x = static_cast<float>(xpos);
		position.y = static_cast<float>(ypos);

		srcRect.x = srcX;
		srcRect.y = srcY;
		srcRect.w = srcRect.h = 128;

		destRect.x = xpos;
		destRect.y = ypos;
		destRect.w = destRect.h = 128;

	}
	void update() override
	{
		destRect.x = static_cast<int>(position.x) - Game::camera.x;
		destRect.y = static_cast<int>(position.y) - Game::camera.y;
	}
	void draw()override
	{
	Draw(texture, srcRect, destRect, SDL_FLIP_NONE,0);
	
	}

};