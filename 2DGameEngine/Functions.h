#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <SDL.h>
#include <string>
#include "Components.h"

// Функція для завантаження текстури з файлу
 SDL_Texture* LoadTexture(const char* texture);
 // Функція для завантаження шрифту і створення текстової текстури
 SDL_Texture* LoadFont(std::string fontPath, int fontSize, const char* fontText);
 // Функція для малювання текстури з можливістю повороту
 void Draw(SDL_Texture* tex, SDL_Rect src, SDL_Rect dest, SDL_RendererFlip flip, int angle);
 // Функція для завантаження мапи з файлу
 void LoadMap(std::string path, int sizeX, int sizeY);
 // Функція для обчислення горизонтальної відстані між центрами двох прямокутників
 float dx(const SDL_Rect & rect1, const SDL_Rect & rect2);
 // Функція для обчислення вертикальної відстані між центрами двох прямокутників
 float dy(const SDL_Rect& rect1, const SDL_Rect& rect2);
 // Функція для обчислення відстані між центрами двох прямокутників
 float distance(SDL_Rect rect1, SDL_Rect rect2);
  // Функція для створення снаряду
 void CreateProjectile(Vector2D pos, Vector2D vel, int range, int speed, int dmg, const char* id, Manager* man, Entity& enemy, int type);
 // Функція для створення ворога
 void CreateEnemy(Vector2D pos, Vector2D vel, int speed, int health, const char* id, Manager* man, int type);
 // Функція для відтворення звуку
 void playSound(const char* path);


#endif
