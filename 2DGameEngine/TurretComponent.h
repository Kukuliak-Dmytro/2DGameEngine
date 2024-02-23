#pragma once

#include <math.h>

#include "Components.h"
#include "ECS.h"
#include "ProjectileComponent.h"
#include "EnemyComponent.h"




class TurretComponent :public Component
{
public:
    SDL_Rect parent;
    
private:
   
public:
    TurretComponent() {

    }
    TurretComponent(int x, int y)
    {
        parent.x = x - Game::camera.x;
        parent.y = y - Game::camera.y;
        parent.h = parent.w = 128;
      
        
    }

  
    void update() override {
      
    }

    

    
    float distance(SDL_Rect rect1, SDL_Rect rect2)
    {
        int x1 = rect1.x + rect1.w / 2;
        int y1 = rect1.y + rect1.h/ 2;
        int x2 = rect2.x + rect2.w / 2;
        int y2 = rect2.y + rect2.h / 2;
        float delta;
        delta = sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
        return delta;
    }
    void inRange(Manager& manager)
    {   
        for(auto e : manager.getGroup(Game::groupEnemies)){
           // std::cout << e->hasComponent<ColliderComponent>();
            //std::cout << e->hasComponent<SpriteComponent>();
           // std::cout << e->hasComponent<EnemyComponent>();
           // 
           // 
           // 
            
           // std::cout << parent.x;
            //std::cout << e->getComponent<ColliderComponent>().collider.x << ":" << e->getComponent<ColliderComponent>().collider.y << std::endl;
          std::cout <<"The distance between turret and enemy is: " << distance(e->getComponent<ColliderComponent>().collider, parent)<< std::endl;
           //проблема у другому аргументі функції



           
        }
    }

};
