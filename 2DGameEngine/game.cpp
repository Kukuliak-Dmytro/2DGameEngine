
#include "Game.h"
#include "TextureManager.h"
#include "Components.h"
#include "MouseControlls.h"
#include "SDL_ttf.h"
#include "EntityManager.h"

Manager manager;
// Рендерер та обробник подій з початку nullptr
SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;

// Визначення прямокутника камери
SDL_Rect Game::camera = { 320,192,640,456 };
int Game::SpawnedEnemies = 0;
int Game::DefeatedEnemies = 0;
int Game::Lives = 10;

// Початкове значення флагу isRunning - false
bool Game::isRunning = false;

// Шлях до файлу карти
const char* mapfile = "assets/tiles.png";
// Групи об'єктів
auto& tilesTrue(manager.getGroup(Game::groupTilesTrue));
auto& tiles(manager.getGroup(Game::groupTilesMap));
auto& turrets(manager.getGroup(Game::groupTurrets));
auto& enemies(manager.getGroup(Game::groupEnemies));
auto& projectiles(manager.getGroup(Game::groupProjectiles));
auto& portals(manager.getGroup(Game::groupPortals));
auto& bases(manager.getGroup(Game::groupBases));

// Визначення екземпляру MouseControls
MouseControlls mouse;

// Конструктор класу Game
Game::Game() {};
// Деструктор класу Game
Game::~Game() {};


// Ініціалізація - налаштування вікна гри, рендерера та інших компонентів
void Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen) {
    int flags = 0;
    if (fullscreen) {
        flags = SDL_WINDOW_FULLSCREEN;
    }
    if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
        std::cout << "Subsystems initialized" << std::endl; // Підсистеми ініціалізовано

        window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
        if (window) {
            std::cout << "Window created " << std::endl; // Створено вікно
        };

        renderer = SDL_CreateRenderer(window, -1, 0);
        if (renderer) {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            std::cout << "Renderer created " << std::endl; // Створено рендерер
        }
        isRunning = true;

    }
    else { isRunning = false; }
    // Ініціалізація SDL_ttf
    if (TTF_Init() == -1) { std::cout << "Error initializing SDL_ttf"; } // Error initializing SDL_ttf
    LoadMap("assets/map.map", 20, 12);

 
    //Adding the portal entity
    auto& portal(manager.addEntity());
    portal.addComponent<TransformComponent>(3*128,5*128,128,128);
    portal.addComponent<SpriteComponent>("assets/portal.png", true);
    portal.addComponent<PortalComponent>(5,5,manager);
    portal.addGroup(Game::groupPortals);
    //Adding the base entity
    auto& base(manager.addEntity());
    base.addComponent<TransformComponent>(12 * 128, 5 * 128, 128, 128);
    base.addComponent<ColliderComponent>();
    base.addComponent<SpriteComponent>("assets/base.png", false);
    base.addComponent<BaseComponent>();
    base.addGroup(Game::groupBases);
};

// Функція обробки подій SDL
void Game::handleEvents() {
    SDL_PollEvent(&event);
    switch (event.type) {
      case SDL_KEYDOWN:
       
        if (event.key.keysym.sym == SDLK_ESCAPE) {   
            isRunning = false;
        }
        break;
       
    default:
        break;
    }
}


// Функція оновлення логіки гри
void Game::update()
{
    manager.refresh();
    manager.update();
    mouse.CameraScroll(camera);
    SDL_RenderClear(renderer);
};

// Функція візуалізації гри
void Game::render() {
    SDL_RenderClear(renderer);
    for (auto& t : tiles) { t->draw(); }
    for (auto& t : tilesTrue) { t->draw(); }
    for (auto& e : enemies) { e->draw(); }
    for (auto& p : projectiles) { p->draw();}
    for (auto& t : turrets) { t->draw(); }
    for (auto& p : portals) { p->draw(); }
    for (auto& b : bases) { b->draw(); }
  
   
    mouse.Hover();
    SDL_RenderPresent(renderer);
};

// Функція очищення ресурсів та виходу з гри
void Game::clean() {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    std::cout << "Game cleaned" << std::endl; // Гра очищена
};

// Функція для додавання плитки до гри
void Game::AddTile(int srcX, int srcY, int xpos, int ypos, bool isInteractive)
{
    auto& tile(manager.addEntity());
    if (isInteractive == true) {
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

// Функція для додавання вежі до гри
void Game::AddTurret(int xpos, int ypos, const char* path1, const char* path2, int type)
{
    for (auto& t : manager.getGroup(Game::groupTurrets)) { if (t->getComponent<TransformComponent>().position.x == xpos && t->getComponent<TransformComponent>().position.y == ypos) t->destroy(); }
    auto& turret(manager.addEntity());
    turret.addComponent<TransformComponent>(xpos, ypos, 128, 128, 1);
    turret.addComponent<TurretComponent>(xpos, ypos, type, path1);
    turret.addComponent<SpriteComponent>(path2, true);
    turret.addGroup(Game::groupTurrets);
   // std::cout << "Turret built"; 
}
