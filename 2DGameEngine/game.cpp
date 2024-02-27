#include "Game.h"
#include "TextureManager.h"
#include "Map.h"
#include "ECS.h"
#include "Components.h"
#include "Vector2D.h"

#include "MouseControlls.h"
#include "SDL_ttf.h"
#include "EntityManager.h"




Map* map;
 
SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;


MouseControlls mouse;
SDL_Rect Game::camera = { 320,192,640,456 };


bool Game::isRunning = false;

auto& player(manager.addEntity());

const char* mapfile = "assets/tiles.png";


auto& tilesTrue(manager.getGroup(Game::groupTilesTrue));
auto& tiles(manager.getGroup(Game::groupTilesMap));
auto& turrets(manager.getGroup(Game::groupTurrets));
auto& enemies(manager.getGroup(Game::groupEnemies));
auto& projectiles(manager.getGroup(Game::groupProjectiles));



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

	player.addComponent<TransformComponent>(4);
	player.addComponent<SpriteComponent>("assets/player_anims.png",true);
	player.addComponent<KeyboardController>();
	player.addComponent<ColliderComponent>("player");
	player.addGroup(Game::groupTurrets);

	//EntityManager::CreateProjectile(Vector2D(600, 600), Vector2D(4, 4), 200, 2, "assets/button1.png", &manager);
	EntityManager::CreateEnemy(Vector2D(-200, 700), Vector2D(1,0), "assets/enemy.png", &manager);
	EntityManager::CreateEnemy(Vector2D(-200, 700), Vector2D(1, 0), "assets/enemy.png", &manager);
	EntityManager::CreateEnemy(Vector2D(-200, 700), Vector2D(1, 0), "assets/enemy.png", &manager);
	EntityManager::CreateEnemy(Vector2D(-200, 600), Vector2D(1, 0), "assets/enemy.png", &manager);
	EntityManager::CreateEnemy(Vector2D(-200, 500), Vector2D(1, 0), "assets/enemy.png", &manager);
	
	//EntityManager::CreateEnemy(Vector2D(6*128, 0), Vector2D(0, 1), "assets/enemy.png", &manager);
	AddTurret(8 * 128, 4 * 128);



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

	
	
		
}	



void Game::render() {
	SDL_RenderClear(renderer);
	for (auto& t : tiles) { t->draw(); }
	for (auto& t : tilesTrue) { t->draw(); }
	for (auto& t : turrets) { t->draw(); }
	for (auto& e : enemies) { e->draw(); }
	for (auto& p : projectiles) { p->draw(); }
	for (auto& t : turrets)
	{
		if (t->hasComponent<TurretComponent>()) { t->getComponent<TurretComponent>().shoot(manager); t->getComponent<SpriteComponent>().setRotation(t->getComponent<TurretComponent>().getDegrees()); }
	}
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
		tile.addGroup(Game::groupTilesTrue);
	}
	else
	{
		tile.addComponent<TileComponent>(srcX, srcY, xpos, ypos, mapfile);
		tile.addGroup(Game::groupTilesMap);
	}
}

void Game::AddTurret(int xpos, int ypos)
{
	auto& turret(manager.addEntity());	
	turret.addComponent<TransformComponent>(xpos,ypos,128,128,1);
	turret.addComponent<dummy>("assets/turret1_base.png");
	turret.addComponent<SpriteComponent>("assets/turret1_anims.png", false);
	turret.addComponent<TurretComponent>(xpos,ypos);
	for (auto& t : tilesTrue) { if (t->getComponent<TileComponent>().position.x == xpos && t->getComponent<TileComponent>().position.y == ypos)t->removeComponent<BuildComponent>(); }
	turret.addGroup(Game::groupTurrets);
	std::cout << "Turret built";
}