
#include <cmath>
#include <fstream>
#include "Functions.h"
#include "Components.h"

void LoadMap(std::string path, int sizeX, int sizeY)
{
	char c;
	std::fstream mapFile;
	mapFile.open(path);

	int srcX; int srcY;

	for (int y = 0; y < sizeY; y++)
	{
		for (int x = 0; x < sizeX; x++) {
			mapFile.get(c);
			srcY = atoi(&c) * 128;
			mapFile.get(c);
			srcX = atoi(&c) * 128;
			if (atoi(&c) == 2) { Game::AddTile(srcX, srcY, x * 128, y * 128, true); }
			else { Game::AddTile(srcX, srcY, x * 128, y * 128, false); }
			mapFile.ignore();

		}
	}
	mapFile.close();
};

 float dx(const SDL_Rect& rect1, const SDL_Rect& rect2) {
     if (&rect2 == nullptr) { return 0.0f; }
     return(rect1.x + (rect1.w / static_cast<float>(2))) - (rect2.x + (rect2.w / static_cast<float>(2)));
}

float dy(const SDL_Rect& rect1, const SDL_Rect& rect2) {
    if (&rect2 == nullptr) { return 0.0f; }
    return (rect1.y + (rect1.h / static_cast<float>(2))) - (rect2.y + (rect2.h / static_cast<float>(2)));  
}
float distance(SDL_Rect rect1, SDL_Rect rect2)
{
    float x1 = static_cast<float>(rect1.x) + static_cast<float>( rect1.w )/ 2;
    float y1 = static_cast<float>(rect1.y) + static_cast<float>(rect1.h) / 2;
    float x2 = static_cast<float>(rect2.x) + static_cast<float>(rect2.w )/ 2;
    float y2 = static_cast<float>(rect2.y) + static_cast<float>(rect2.h )/ 2;
    float delta;
    delta = sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
    return delta;
}
