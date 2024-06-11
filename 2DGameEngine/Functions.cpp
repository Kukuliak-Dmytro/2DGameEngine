
#include "Components.h"
#include "Functions.h"
#include <cmath>
#include <fstream>
#include <string>


SDL_Texture* LoadTexture(const char* texture) {
	SDL_Surface* tempSurface = IMG_Load(texture);
	SDL_Texture* tex = SDL_CreateTextureFromSurface(Game::renderer, tempSurface);
	SDL_FreeSurface(tempSurface);

	return tex;



}
SDL_Texture* LoadFont(std::string fontPath, int fontSize, const char* fontText) {
	TTF_Font* font = TTF_OpenFont(fontPath.c_str(), fontSize);
	SDL_Color color = { 128,128,128,255 };
	SDL_Surface* tempSurface = TTF_RenderText_Blended(font, fontText, color);
	SDL_Texture* tex = SDL_CreateTextureFromSurface(Game::renderer, tempSurface);
	SDL_FreeSurface(tempSurface);

	return tex;



}
void Draw(SDL_Texture* tex, SDL_Rect src, SDL_Rect dest, SDL_RendererFlip flip, int angle)
{
	SDL_RenderCopyEx(Game::renderer, tex, &src, &dest, angle, NULL, flip);
}

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
	float x1 = static_cast<float>(rect1.x) + static_cast<float>(rect1.w) / 2;
	float y1 = static_cast<float>(rect1.y) + static_cast<float>(rect1.h) / 2;
	float x2 = static_cast<float>(rect2.x) + static_cast<float>(rect2.w) / 2;
	float y2 = static_cast<float>(rect2.y) + static_cast<float>(rect2.h) / 2;
	float delta;
	delta = static_cast<float>(sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2)));
	return delta;
}

void CreateProjectile(Vector2D pos, Vector2D vel, int range, int speed, int dmg, const char* id, Manager* man, Entity& enemy, int type)
{
	//Creating using the manager
	auto& projectile(man->addEntity());
	//Passing the position size, and speed
	projectile.addComponent<TransformComponent>(pos.x, pos.y, 16, 16, speed);
	//Passing the path and whether is animated
	projectile.addComponent<SpriteComponent>(id, false);
	//Passing range, speed. velocity, damage, target, ane the color of the projectile
	projectile.addComponent<ProjectileComponent>(range, speed, vel, dmg, enemy, type);
	//Creating the collider
	projectile.addComponent<ColliderComponent>();
	//Adding to the group
	projectile.addGroup(Game::groupProjectiles);

}

//Creating an enemy
void CreateEnemy(Vector2D pos, Vector2D vel, int speed, int health, const char* id, Manager* man, int type)
{
	//Creating using the manager
	auto& enemy(man->addEntity());
	//Passing the position, size and speed
	enemy.addComponent<TransformComponent>(pos.x, pos.y, 32, 32, speed);
	//Passing the path and whether is animated
	enemy.addComponent<SpriteComponent>(id, false);
	//Passing the health, velocity, and color of the enemy
	enemy.addComponent<EnemyComponent>(health, vel, type);
	//Adding the collider
	//It is initialized in its init() function
	enemy.addComponent<ColliderComponent>();
	//Adding to the group
	enemy.addGroup(Game::groupEnemies);

}

void playSound(const char* path) {
	Mix_Chunk* sound = Mix_LoadWAV(path);
	if (sound == NULL) {
		std::cout << "Failed to load sound effect! SDL_mixer Error: " << Mix_GetError();
	}
	Mix_PlayChannel(-1, sound, 0);
	//Mix_FreeChunk(sound);

}


void playMusic(const char* path) {
	Mix_Music* music = Mix_LoadMUS(path);
	if (music == NULL) {
		std::cout << "Failed to load music! SDL_mixer Error: " << Mix_GetError();
	}
	Mix_PlayMusic(music, -1);
	//Mix_FreeMusic(music);
}