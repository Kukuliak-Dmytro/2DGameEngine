#pragma once
#include "Components.h"
#include "Map.h"
#include "TextureManager.h"
#include <sstream>
#include "Game.h"

// ���������� ���������� ���������


// ���� ��� ��������� �����
class MouseControlls
{
public:
	// ��������� ��� ������� �� ����
	struct GridPosition {
		int x, y;
	} position;

	// ������������ ��� ����������� �������
	SDL_Rect src, dest;

	// �������� ��� ������� ����
	SDL_Texture* hoverTexture;
	SDL_Texture* test;

	// �������� �� ������ ������� ����
	SDL_Point prevPos;
	SDL_Point mousePos;

	// ����������� �� �������������
	MouseControlls() = default;

	// ������� ��� ��������� ������ �� ��������� ����
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

	// ������� ��� ��������� ������� �� ����
	GridPosition getGrid() {
		return position;
	}

	// ������� ��� ����������� ������� ����
	void Hover() {
		SDL_GetMouseState(&mousePos.x, &mousePos.y);

		// ���������� ������� �� ����
		position.x = ((mousePos.x + Game::camera.x) / 128);
		position.y = ((mousePos.y + Game::camera.y) / 128);

		// ������������ ������������
		src.x = src.y = 0;
		src.w = src.h = 128;

		dest.w = dest.h = 128;
		dest.x = position.x * 128 - Game::camera.x;
		dest.y = position.y * 128 - Game::camera.y;

		// ������������ �������� ��� �������
		hoverTexture = TextureManager::LoadTexture("assets/hover.png");

		// ³���������� �������
		TextureManager::Draw(hoverTexture, src, dest, SDL_FLIP_NONE, 0);
	}
};
