#include "Game.h"
#include "TextureManager.h"
#include "Map.h"
#include "ECS.h"
#include "Components.h"
#include "Vector2D.h"

#include "MouseControlls.h"
#include "SDL_ttf.h"
#include "EntityManager.h"

// Вказівник на об'єкт класу Map
Map* map;

// Рендерер та обробник подій з початку nullptr
SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;

// Визначення прямокутника камери
SDL_Rect Game::camera = { 320,192,640,456 };

// Початкове значення флагу isRunning - false
bool Game::isRunning = false;

// Визначення об'єкту player
auto& player(manager.addEntity());

// Шлях до файлу карти
const char* mapfile = "assets/tiles.png";

// Групи об'єктів
auto& tilesTrue(manager.getGroup(Game::groupTilesTrue));
auto& tiles(manager.getGroup(Game::groupTilesMap));
auto& turrets(manager.getGroup(Game::groupTurrets));
auto& enemies(manager.getGroup(Game::groupEnemies));
auto& projectiles(manager.getGroup(Game::groupProjectiles));

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
    if (TTF_Init() == -1) { std::cout << "Помилка ініціалізації SDL_ttf"; } // Error initializing SDL_ttf
    map = new Map();
    Map::LoadMap("assets/map.map", 20, 12);

    // Ініціалізація гравця
    player.addComponent<TransformComponent>(4);
    player.addComponent<SpriteComponent>("assets/player_anims.png", true);
    player.addComponent<KeyboardController>();
    player.addComponent<ColliderComponent>("player");
    player.addGroup(Game::groupTurrets);

  //  EntityManager::CreateEnemy(Vector2D(0, 500), Vector2D(1, 0), "assets/enemy.png", &manager);
    EntityManager::CreateEnemy(Vector2D(500, 0), Vector2D(0, 1), "assets/enemy.png", &manager);

    AddTurret(8 * 128, 4 * 128);
};

// Функція обробки подій SDL
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
    for (auto& t : turrets) { t->draw(); }
    for (auto& e : enemies) { e->draw(); }
    for (auto& p : projectiles) { p->draw(); }
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
void Game::AddTurret(int xpos, int ypos)
{
    auto& turret(manager.addEntity());
    turret.addComponent<TransformComponent>(xpos, ypos, 128, 128, 1);
    turret.addComponent<dummy>("assets/turret1_base.png");
    turret.addComponent<SpriteComponent>("assets/turret1_anims.png", true);
    turret.addComponent<TurretComponent>(xpos, ypos);
    for (auto& t : tilesTrue) { if (t->getComponent<TileComponent>().position.x == xpos && t->getComponent<TileComponent>().position.y == ypos)t->removeComponent<BuildComponent>(); }
    turret.addGroup(Game::groupTurrets);
    std::cout << "Turret built"; // Turret built
}
