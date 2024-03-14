#ifndef TOOLS_H
#define TOOLS_H

#include <SDL.h>
class tools{
public:

static float dx(const SDL_Rect & rect1, const SDL_Rect & rect2);
static float dy(const SDL_Rect& rect1, const SDL_Rect& rect2);
static float distance(SDL_Rect rect1, SDL_Rect rect2);
private:

};
#endif
