#pragma once

#include <math.h>
 
#include "Components.h"
#include "ECS.h"
#include "ProjectileComponent.h"
#include "EnemyComponent.h"
#include "EntityManager.h"




class TurretComponent :public Component
{    
private:
    SDL_Rect parent;
    SDL_Point A0;
    float angleDegrees;
    int shootDelay = 1000;

public:
    TurretComponent() {

    }
    TurretComponent(int x, int y)
    {
        parent.x = x - Game::camera.x;
        parent.y = y - Game::camera.y;
        parent.h = parent.w = 128;
        A0.x = x + 64 / 2;
        A0.y = y + 64 / 2;
      
        
    }

  
    void update() override {
        

    }    

    
    float distance(SDL_Rect rect1, SDL_Rect rect2)
    {
        float x1 = rect1.x + rect1.w / 2;
        float y1 = rect1.y + rect1.h / 2;
        float x2 = rect2.x + rect2.w / 2;
        float y2 = rect2.y + rect2.h / 2;
        float delta;
        delta = sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
        return delta;
    }


    float dx(SDL_Rect& rect)
    {

        return (rect.x+(rect.w/2)) - A0.x;
    }

    float dy(SDL_Rect& rect)
    {
        return (rect.y + (rect.h / 2)) - A0.y;
    }
    
    float& getDegrees() { return angleDegrees; }

    void shoot(Manager& manager)
    {
        
        auto& e = manager.getGroup(Game::groupEnemies).front();
          
                if (distance(e->getComponent<ColliderComponent>().collider, parent) < 700) {
                    float dxLength = dx(e->getComponent<ColliderComponent>().collider);
                    float dyLength = dy(e->getComponent<ColliderComponent>().collider);

                    float angleRadians = atan2(dyLength, dxLength);
                    angleDegrees = angleRadians * 180.0f / M_PI;

                    if (angleDegrees < 0) {
                        angleDegrees += 360.0f;
                    }

                    if (shootDelay >= 1000) {
                        EntityManager::CreateProjectile(Vector2D(A0.x, A0.y), Vector2D(dxLength / 100, dyLength / 100), 200, 5, "assets/button1.png", &manager);
                        shootDelay = 0;

                    }
                    shootDelay = shootDelay + 100;
                }
            

        
    }




};
