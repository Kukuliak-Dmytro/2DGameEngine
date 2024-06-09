#pragma once
#include <vector>
#include <ctime>

#include "Components.h"

class PortalComponent : public Component {
private:
    int enemyDelay = 1000;
    Vector2D spawnpoint;
    Vector2D path;
    Manager& managerial;
	int waves, nEnemies;
    int k = 0;
    int enemytype = 1;
    int spawnedEnemies = 0;
    SDL_Rect src, dest;
    int enemySpeed = 1;
    int enemyHealth = 100;
    std::chrono::steady_clock::time_point lastShotTime;
  public:
	PortalComponent() = default;
	PortalComponent(int w, int e, Manager& man) :waves(w), nEnemies(e), managerial(man) { };
      
    void update() override {
        srand(time(NULL));
        enemytype = rand() % 4 + 1;
        entity->getComponent<SpriteComponent>().Play("Portal", 400);
        spawnpoint.x = static_cast<float>(entity->getComponent<SpriteComponent>().getRect().x + Game::camera.x+rand() %  50 + 1);
        spawnpoint.y = static_cast<float>(entity->getComponent<SpriteComponent>().getRect().y + Game::camera.y+rand() % 80 + 1);
        path.x = 1.5;
        path.y = 0;
        summonWave();
    }
    void summonWave() {   
      auto currentTime = std::chrono::steady_clock::now();
       std::chrono::duration<double, std::milli> elapsedTime = currentTime - lastShotTime;
       if (elapsedTime.count() >= enemyDelay) {
           std::string enemyFileName = "assets/enemy" + std::to_string(enemytype) + ".png";
            CreateEnemy(spawnpoint, path,enemySpeed,enemyHealth, enemyFileName.c_str(), &managerial,enemytype);
            enemyDelay--;
            lastShotTime = std::chrono::steady_clock::now();
            Game::SpawnedEnemies++;
                         
       }    
        
    }
    void draw() override {
        std::string file = "Enemies spawned " + std::to_string(Game::SpawnedEnemies);
        const char* a = file.c_str();
      
        src.x = src.y = 0; 
        src.h = src.w = 512;
        dest.w = 312;
        dest.h = 64;
        dest.x = 208;
        dest.y = 128;


       Draw(LoadFont("assets/font2.ttf",24, a), src, dest, SDL_FLIP_NONE, 0);

        file = "Enemies defeated " + std::to_string(Game::DefeatedEnemies);
         a = file.c_str();

        src.x = src.y = 0; 
        src.h = src.w = 512;
        dest.w = 312;
        dest.h = 64;
        dest.x = 208;
        dest.y = 192;


       Draw(LoadFont("assets/font2.ttf", 24, a), src, dest, SDL_FLIP_NONE, 0);
    }

	
	
};
