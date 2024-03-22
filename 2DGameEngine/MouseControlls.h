#pragma once
#include "Components.h"
#include "Map.h"
#include "TextureManager.h"
#include <sstream>
#include "Game.h"



//Class for mouse controlls
class MouseControlls
{
public:
	// THe structure represents the position of the mouse on the tilemap
	struct GridPosition {
		int x, y;
	} position;

	//For drawing textures
	SDL_Rect src, dest;
	SDL_Texture* hoverTexture;
	SDL_Texture* test;

	// Previous and current mouse positions
	SDL_Point prevPos;
	SDL_Point mousePos;

	MouseControlls() = default;

	// The function that is used to scroll the screen
	void CameraScroll(SDL_Rect& camera) {
		//SDL function that returns the posiotion and button states of the mouse
		SDL_GetMouseState(&mousePos.x, &mousePos.y);
		//if the left button is pressed
		if (SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON(SDL_BUTTON_LEFT)) {
			//dynamically change the camera position based on the moving of the mouse
			int relX = mousePos.x - prevPos.x;
			int relY = mousePos.y - prevPos.y;
			//move the camerain the opposite direction to the mouse movements
			camera.x -= relX;
			camera.y -= relY;
		}
		//Stores the previous position of the mouse
		prevPos = mousePos;

		//COnditions that establish the borders of the game
		if (camera.x < 0) camera.x = 0;
		if (camera.y < 0) camera.y = 0;
		if (camera.x > camera.w) camera.x = camera.w;
		if (camera.y > camera.h) camera.y = camera.h;
	}

	// Returns the grid position
	GridPosition getGrid() {
		return position;
	}

	// The method that will render a square an the mouse location
	void Hover() {
		
		SDL_GetMouseState(&mousePos.x, &mousePos.y);

		//Calculates the grid position
		position.x = ((mousePos.x + Game::camera.x) / 128);
		position.y = ((mousePos.y + Game::camera.y) / 128);

		//For rendering
		src.x = src.y = 0;
		src.w = src.h = 128;
		dest.w = dest.h = 128;
		dest.x = position.x * 128 - Game::camera.x;
		dest.y = position.y * 128 - Game::camera.y;

		// Loading the texture into the variable
		hoverTexture = TextureManager::LoadTexture("assets/hover.png");
		// Drawing the hover square
		TextureManager::Draw(hoverTexture, src, dest, SDL_FLIP_NONE, 0);
	}
};
