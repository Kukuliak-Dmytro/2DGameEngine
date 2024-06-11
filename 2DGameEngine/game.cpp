
#include "Game.h"
#include "Components.h"
#include "MouseControlls.h"



Manager manager;
// Рендерер та обробник подій з початку nullptr
SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;

// Визначення прямокутника камери
SDL_Rect Game::camera = { 320,192,640,384 };
int Game::SpawnedEnemies = 0;
int Game::DefeatedEnemies = 0;
int Game::Lives = 10;

// Початкове значення флагу isRunning - false
bool Game::isRunning = false;
bool Game::pauseSwitch = true;

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

    //Loading the map
    LoadMap("assets/map.map", 20, 12);   
    // Ініціалізація SDL_ttf
    if (TTF_Init() == -1) { std::cout << "Error initializing SDL_ttf"; } // Error initializing SDL_ttf
   
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
      
    }
    playSound("assets/acepe.wav");
 

 
    //Adding the portal entity
    auto& portal(manager.addEntity());
    portal.addComponent<TransformComponent>(3*128,5*128,128,128);
    portal.addComponent<SpriteComponent>("assets/portal.png", true);
    portal.addComponent<PortalComponent>(manager);
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
        if (event.key.keysym.sym == SDLK_SPACE && Lives==0) {
            DefeatedEnemies = 0;
            SpawnedEnemies = 0;
            Lives = 10;
            Game::pauseSwitch = true;
            for (auto e : enemies){ e->destroy();}
            for (auto t : turrets){ t->destroy();}
            for (auto p : projectiles){ p->destroy();}

           
        }
        break;
      
    default:
        break;
    }
}


// Функція оновлення логіки гри
void Game::update()
{
    if (Game::pauseSwitch == true) 
    {
        manager.refresh();
        manager.update();
        mouse.CameraScroll(camera);
    }
     
    if (Game::Lives == 0)Game::pauseSwitch = false;
    
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
    if (Game::pauseSwitch == false)
    {        
            SDL_Rect src, dest;
            src.x = src.y = 0; src.h = src.w = 512;
            dest.w = 500;
            dest.h = 128;
            dest.x = 700;
            dest.y = 500;
            const char* over = "Game over";
            Draw(LoadFont("assets/font2.ttf", 8, over), src, dest, SDL_FLIP_NONE, 0);           
            std::string file = "Score: " + std::to_string(Game::DefeatedEnemies);
            const char* score = file.c_str();
            dest.w = 256;
            dest.h = 64;
            dest.x = 800;
            dest.y = 658;
            Draw(LoadFont("assets/font2.ttf", 8, score), src, dest, SDL_FLIP_NONE, 0);
        
    }
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
