#ifndef Game_h
#define Game_h

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <vector>
#include <SDL_ttf.h>
#include "ProjectileManager.h"

class ColliderComponent;

class Game {
public:
	Game();
	~Game();

	void init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);

	void handleEvents();
	void update();
	bool running() { return isRunning; };
	void render();
	void clean();
	
	static void AddTile(int srcX, int srcY, int xpos, int ypos, bool isInteractive);
	static void AddTurret( int xpos, int ypos);
	static SDL_Renderer* renderer;
	static SDL_Event event;
	static std::vector<ColliderComponent*> colliders;
	static bool isRunning;
	static SDL_Rect camera;
	
	enum groupLabels : std::size_t
	{
		groupTilesMap,
		groupTilesTrue,
		groupTurrets,
		groupEnemies,
		groupProjectiles
	};
private:
	int count = 0;
	
	SDL_Window* window;


};
#endif