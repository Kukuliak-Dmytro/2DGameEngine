#include "Tools.h"
#include "Components.h"
#include <cmath> // Include <cmath> for mathematical functions

 float tools::dx(const SDL_Rect& rect1, const SDL_Rect& rect2) {
     if (&rect2 == nullptr) { return 0.0f; }
     return(rect1.x + (rect1.w / static_cast<float>(2))) - (rect2.x + (rect2.w / static_cast<float>(2)));
}

float tools::dy(const SDL_Rect& rect1, const SDL_Rect& rect2) {
    if (&rect2 == nullptr) { return 0.0f; }
    return (rect1.y + (rect1.h / static_cast<float>(2))) - (rect2.y + (rect2.h / static_cast<float>(2)));  
}
float tools::distance(SDL_Rect rect1, SDL_Rect rect2)
{
    float x1 = static_cast<float>(rect1.x) + static_cast<float>( rect1.w )/ 2;
    float y1 = static_cast<float>(rect1.y) + static_cast<float>(rect1.h) / 2;
    float x2 = static_cast<float>(rect2.x) + static_cast<float>(rect2.w )/ 2;
    float y2 = static_cast<float>(rect2.y) + static_cast<float>(rect2.h )/ 2;
    float delta;
    delta = sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
    return delta;
}
