#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <SDL.h>
#include <string>
#include "Components.h"

 SDL_Texture* LoadTexture(const char* texture);
 SDL_Texture* LoadFont(std::string fontPath, int fontSize, const char* fontText);
 void Draw(SDL_Texture* tex, SDL_Rect src, SDL_Rect dest, SDL_RendererFlip flip, int angle);
 void LoadMap(std::string path, int sizeX, int sizeY);
 float dx(const SDL_Rect & rect1, const SDL_Rect & rect2);
 float dy(const SDL_Rect& rect1, const SDL_Rect& rect2);
 float distance(SDL_Rect rect1, SDL_Rect rect2);
 void CreateProjectile(Vector2D pos, Vector2D vel, int range, int speed, int dmg, const char* id, Manager* man, Entity& enemy, int type);
 void CreateEnemy(Vector2D pos, Vector2D vel, int speed, int health, const char* id, Manager* man, int type);
 void playSound(const char* path);
 void playMusic(const char* path);

#endif
