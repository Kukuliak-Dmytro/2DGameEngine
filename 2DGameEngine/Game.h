#ifndef Game_h
#define Game_h

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <vector>
#include <SDL_ttf.h>

// Class definition for the Game
class Game {
public:
	// Constructor
	Game();
	// Destructor
	~Game();

	// Initializes the game
	void init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);

	// Handles events such as user input
	void handleEvents();

	// Updates game state
	void update();

	// Checks if the game is running
	bool running() { return isRunning; };

	// Renders the game
	void render();

	// Cleans up resources and quits the game
	void clean();

	// Static function to add a tile to the game
	static void AddTile(int srcX, int srcY, int xpos, int ypos, bool isInteractive);

	// Static function to add a turret to the game
	static void AddTurret(int xpos, int ypos);

	// Static renderer for the game
	static SDL_Renderer* renderer;

	// Static event handler for the game
	static SDL_Event event;

	// Boolean indicating if the game is running
	static bool isRunning;

	// Camera rectangle for rendering
	static SDL_Rect camera;

	// Enum defining groups of game objects
	enum groupLabels : std::size_t
	{
		groupTilesMap,
		groupTilesTrue,
		groupTurrets,
		groupEnemies,
		groupProjectiles,
		groupPortals
	};
private:

	// SDL window for the game
	SDL_Window* window;
};

#endif
