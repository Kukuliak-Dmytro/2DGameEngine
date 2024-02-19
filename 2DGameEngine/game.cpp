#include "Game.h"
#include "TextureManager.h"
#include "Map.h"
#include "ECS.h"
#include "Components.h"
#include "Vector2D.h"
#include "Collision.h"
#include "MouseControlls.h"
#include "SDL_ttf.h"




Map* map;
 extern Manager manager;

SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;

MouseControlls mouse;
SDL_Rect Game::camera = { 320,192,640,456 };

std::vector<ColliderComponent*> Game::colliders;

bool Game::isRunning = false;

auto& player(manager.addEntity());

const char* mapfile = "assets/tiles.png";

enum groupLabel : std::size_t
{
	groupTilesMap,
	groupTilesTrue, 
	groupTurrets, 
	groupEnemies
};
auto& tilesTrue(manager.getGroup(groupTilesTrue));
auto& tiles(manager.getGroup(groupTilesMap));
auto& turrets(manager.getGroup(groupTurrets));
auto& enemies(manager.getGroup(groupEnemies));



Game::Game()
{};
Game:: ~Game()
{};

void Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen) {
	int flags = 0;
	if (fullscreen) {
		flags = SDL_WINDOW_FULLSCREEN;
	}
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
		std::cout << "Subsystems initialized" << std::endl;

		window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
		if (window) {
			std::cout << "Window created " << std::endl;

		};

		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer) {
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			std::cout << "Renderer created " << std::endl;
		}
		isRunning = true;
		

	}
	else { isRunning = false; }

	if (TTF_Init() == -1) { std::cout << "Error initializing SDL_ttf"; }

	map = new Map();
	Map::LoadMap("assets/map.map", 20, 12);

	player.addComponent<TransformComponent>(5);
	player.addComponent<SpriteComponent>("assets/player_anims.png",true);
	player.addComponent<KeyboardController>();
	player.addComponent<ColliderComponent>("player");
	player.addGroup(groupTurrets);


};

void Game::handleEvents() {
	SDL_PollEvent(&event);
	switch (event.type) {
	case SDL_QUIT:
		isRunning = false;
		break;

	default:
		break;
	}
};

void Game::update()
{
	manager.refresh();
	manager.update();
	mouse.CameraScroll(camera);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black
	SDL_RenderClear(renderer);

	bullshit code that wont work
	
		
}	



void Game::render() {
	SDL_RenderClear(renderer);
	for (auto& t : tiles) { t->draw(); }
	for (auto& t : tilesTrue) { t->draw(); }
	for (auto& t : turrets) { t->draw(); }
	for (auto& e : enemies) { e->draw(); }
	mouse.Hover();
	SDL_RenderPresent(renderer);
	
};

void Game::clean() {
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	std::cout << "Game cleaned" << std::endl;

};

void Game::AddTile(int srcX, int srcY, int xpos, int ypos,bool isInteractive)
{
	auto& tile(manager.addEntity());
	if (isInteractive==true) {
		tile.addComponent<TileComponent>(srcX, srcY, xpos, ypos, mapfile);
		tile.addComponent<BuildComponent>(xpos, ypos);
		tile.addGroup(groupTilesTrue);
	}
	else
	{
		tile.addComponent<TileComponent>(srcX, srcY, xpos, ypos, mapfile);
		tile.addGroup(groupTilesMap);
	}
}

void Game::AddTurret(int xpos, int ypos)
{
	auto& turret(manager.addEntity());	
	turret.addComponent<TransformComponent>(xpos,ypos,128,128,1);
	turret.addComponent<SpriteComponent>("assets/turret1.png", false);
	turret.addComponent<Button>();
	for (auto& t : tilesTrue) { if (t->getComponent<TileComponent>().position.x == xpos && t->getComponent<TileComponent>().position.y == ypos)t->removeComponent<BuildComponent>(); }
	turret.addGroup(groupTurrets);
	std::cout << "Turret built";
}