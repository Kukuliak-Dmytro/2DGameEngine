#pragma once
#include <vector>
#include <ctime>
#include <thread>
#include "Components.h"

class PortalComponent : public Component {
private:
    Vector2D spawnpoint;
    Vector2D path;
    Manager& managerial;
	int waves, nEnemies;
    int k = 0;
    std::chrono::steady_clock::time_point lastShotTime;
  public:
	PortalComponent() = default;
	PortalComponent(int w, int e, Manager& man) :waves(w), nEnemies(e), managerial(man) { };
      
    void update() override {
        entity->getComponent<SpriteComponent>().Play("Portal", 400);
        srand(time(NULL)); 
        spawnpoint.x = entity->getComponent<SpriteComponent>().getRect().x + Game::camera.x+rand() %  50 + 1;
        spawnpoint.y = entity->getComponent<SpriteComponent>().getRect().y + Game::camera.y+rand() % 80 + 1;
        path.x = 0.5;
        path.y = 0;
        summonWave();
    }
    void summonWave() {   
      auto currentTime = std::chrono::steady_clock::now();
       std::chrono::duration<double, std::milli> elapsedTime = currentTime - lastShotTime;
       if (elapsedTime.count() >= 1000) {
            EntityManager::CreateEnemy(spawnpoint, path, "assets/enemy1.png", &managerial);
            lastShotTime = std::chrono::steady_clock::now();
                         
       }    
        
    }

	
	
};
