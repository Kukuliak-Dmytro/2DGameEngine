#pragma once
#include "Components.h"
#include "Map.h"
#include "TextureManager.h"
#include <sstream>
#include "Game.h"

// Оголошення екземпляру менеджера


// Клас для управління мишою
class MouseControlls
{
public:
	// Структура для позиції на сітці
	struct GridPosition {
		int x, y;
	} position;

	// Прямокутники для відображення текстур
	SDL_Rect src, dest;

	// Текстури для підказок миші
	SDL_Texture* hoverTexture;
	SDL_Texture* test;

	// Попередні та поточні позиції миші
	SDL_Point prevPos;
	SDL_Point mousePos;

	// Конструктор за замовчуванням
	MouseControlls() = default;

	// Функція для прокрутки камери за допомогою миші
	void CameraScroll(SDL_Rect& camera) {
		SDL_GetMouseState(&mousePos.x, &mousePos.y);

		if (SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON(SDL_BUTTON_LEFT)) {
			int relX = mousePos.x - prevPos.x;
			int relY = mousePos.y - prevPos.y;
			camera.x -= relX;
			camera.y -= relY;
		}
		prevPos = mousePos;

		if (camera.x < 0) camera.x = 0;
		if (camera.y < 0) camera.y = 0;
		if (camera.x > camera.w) camera.x = camera.w;
		if (camera.y > camera.h) camera.y = camera.h;
	}

	// Функція для отримання позиції на сітці
	GridPosition getGrid() {
		return position;
	}

	// Функція для відображення підказки миші
	void Hover() {
		SDL_GetMouseState(&mousePos.x, &mousePos.y);

		// Обчислення позиції на сітці
		position.x = ((mousePos.x + Game::camera.x) / 128);
		position.y = ((mousePos.y + Game::camera.y) / 128);

		// Налаштування прямокутників
		src.x = src.y = 0;
		src.w = src.h = 128;

		dest.w = dest.h = 128;
		dest.x = position.x * 128 - Game::camera.x;
		dest.y = position.y * 128 - Game::camera.y;

		// Завантаження текстури для підказки
		hoverTexture = TextureManager::LoadTexture("assets/hover.png");

		// Відображення підказки
		TextureManager::Draw(hoverTexture, src, dest, SDL_FLIP_NONE, 0);
	}
};
