#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <SDL.h>
#include <string>
 void LoadMap(std::string path, int sizeX, int sizeY);
 float dx(const SDL_Rect & rect1, const SDL_Rect & rect2);
 float dy(const SDL_Rect& rect1, const SDL_Rect& rect2);
 float distance(SDL_Rect rect1, SDL_Rect rect2);



#endif
