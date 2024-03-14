#pragma once

#include <math.h>
#include <chrono>
#include "Components.h"
#include "ECS.h"
#include "ProjectileComponent.h"
#include "EnemyComponent.h"
#include "EntityManager.h"
#include "Tools.h"




class TurretComponent :public Component
{    
private:
    //Rectangle that holds the position of the turret
    SDL_Rect parent;
    //The central point, holds the position where the projectile would be initialized
    SDL_Point A0;
    //The angle of turret rotation
    float angleDegrees;
    //The delay between shots
    // 1000 milliseconds = 1 second
    const int shootDelayDuration = 450; 
    //Chrono to measure time between shots
    std::chrono::steady_clock::time_point lastShotTime;

public:
    TurretComponent() {

    }
    TurretComponent(int x, int y)
    {
     
        parent.x = x;
        parent.y = y;
        parent.h = parent.w = 128;
        A0.x = x + 128 / 2;
        A0.y = y + 128 / 2;
       
        
    } 
 
    //Method that checks enemies within range and automatically shoots
    void shoot(Manager& manager)
    {
        //Getting the groupEnemies vector and checking !empty, otherwisw - crash
        if (!manager.getGroup(Game::groupEnemies).empty()) {
            //Aiming at the first enemy in the vector
            auto& e = manager.getGroup(Game::groupEnemies).front();
            //Measures elapsed time until the required "shootDelayDuration" has passed.
            //It is here because the turret must reload evem when there aren`t enemies in range
            auto currentTime = std::chrono::steady_clock::now();
            std::chrono::duration<double, std::milli> elapsedTime = currentTime - lastShotTime;
            //If in range
            if (tools::distance(e->getComponent<ColliderComponent>().collider, parent) < 700) 
            {
                //Calculating horisontal and vertical distances using trigonometry
                float dxLength = tools::dx(e->getComponent<ColliderComponent>().collider, parent);
                float dyLength = tools::dy(e->getComponent<ColliderComponent>().collider, parent);
                //and setting the direction of the vector according to those values
                Vector2D direction(dxLength, dyLength);
                //Normalizing the vector = keeps the same direction, but its modulus is set to 1
                direction.normalize();       
                //Calculating angle in radians adn converting to degrees
                float angleRadians = atan2(dyLength, dxLength);
                angleDegrees = angleRadians * 180.0f / M_PI;
                
                //If the required time has passed            
                if (elapsedTime.count() >= shootDelayDuration) {
                    //Create projectile and call animation
                    EntityManager::CreateProjectile(Vector2D(A0.x, A0.y), direction, 500, 3, 10, "assets/button1.png", &manager, *e);
                    lastShotTime = std::chrono::steady_clock::now();
                    entity->getComponent<SpriteComponent>().Play("Shoot", shootDelayDuration);
                }             
            }
            else entity->getComponent<SpriteComponent>().Play("Idle");
                       
        }
        else entity->getComponent<SpriteComponent>().Play("Idle");
    }

    void update() override {
        //Calling shoot() on a regular basis
        entity->getComponent<TurretComponent>().shoot(entity->getManager());
        //Setting the rotation using a setter because the render is in <SpriteComponent>
        entity->getComponent<SpriteComponent>().setRotation(angleDegrees);

    }

};
