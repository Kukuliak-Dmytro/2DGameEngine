#include "Map.h"
#include "TextureManager.h"
#include "Game.h"
#include <fstream>

Map::Map() {
	
};

Map::~Map() {

}
void Map::LoadMap(std::string path, int sizeX, int sizeY) 
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
			if(atoi(&c)==2){Game::AddTile(srcX, srcY, x * 128, y * 128, true); }
			else { Game::AddTile(srcX, srcY, x * 128, y * 128, false); }
			mapFile.ignore();

					}
	}
	mapFile.close();
};
