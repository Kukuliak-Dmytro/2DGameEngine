#pragma once
#include "Components.h"

class InteractComponent :public TileComponent {
public:
	SDL_Rect t1, t2, t3, t4;
	InteractComponent() {
		t1.h = t1.w = 64;
		t1.x = position.x + 64;
		t1.y = position.y + 64;
	}

	void draw() override {
		texture = TextureManager::LoadTexture("assets/terrain_ss.png");
		TextureManager::Draw(texture, t2, t1, SDL_FLIP_NONE);
	}
};
