#pragma once

#include <math.h>
#include <chrono>
#include "Game.h"
#include "EnemyComponent.h"
#include"ProjectileComponent.h"







class TurretComponent :public Component
{    
private:
    //Rectangle that holds the position of the turret
    SDL_Rect parent;
    //The central point, holds the position where the projectile would be initialized
    SDL_Point A0;
    //The variables for calculating the angle of rotation
    float rotationDifference = 0;
    float currentRotation=0;
    float targetRotation=0;
    float rotationSpeed=0.1f;
  
    //The specs of the turret
    const int range = 700;
    // 1000 milliseconds = 1 second
    const int shootDelayDuration = 450; 
    const int projectileRange = 250;
    const int projectileSpeed= 3;
    const int projectileDamage= 20;
    int turretColor = 1;
    //The resources for rendering the base of the turret
    SDL_Texture* texture;
    SDL_Rect srcRect, destRect;

    //Chrono to measure time between shots
    std::chrono::steady_clock::time_point lastShotTime;

public:
    TurretComponent() {

    }
    TurretComponent(int x, int y, int color, const char* path)
    {
        //For rendering the base of the turret
        texture = LoadTexture(path);
        srcRect.x = srcRect.y = 0;
        srcRect.w = 128;
        srcRect.h = 128;
        //Parent is the position of the turret used for calculating the distance to the enemy
        parent.x = x;
        parent.y = y;
        parent.h = parent.w = 128;
        A0.x = x + 128 / 2;
        A0.y = y + 128 / 2;
        turretColor = color;
        
    } 
 
    //Method that checks enemies within range and automatically shoots
    void shoot(Manager& manager)
    {
        //Getting the groupEnemies vector and checking !empty, otherwise - crash
        if (!manager.getGroup(Game::groupEnemies).empty()) {
            for (auto& e : manager.getGroup(Game::groupEnemies)) {
                if (e->getComponent<EnemyComponent>().enemyColor == turretColor) {
                    //Measures elapsed time until the required "shootDelayDuration" has passed.
                     //It is here because the turret must reload evem when there aren`t enemies in range
                    auto currentTime = std::chrono::steady_clock::now();
                    std::chrono::duration<double, std::milli> elapsedTime = currentTime - lastShotTime;

                    // Check if the enemy is in range
                    if (distance(e->getComponent<ColliderComponent>().collider, parent) < range) {
                        //Calculating horisontal and vertical distances using trigonometry
                        float dxLength = dx(e->getComponent<ColliderComponent>().collider, parent);
                        float dyLength = dy(e->getComponent<ColliderComponent>().collider, parent);
                        //and setting the direction of the vector according to those values
                        Vector2D direction(dxLength, dyLength);
                        //Normalizing the vector = keeps the same direction, but its modulus(length) is set to 1
                        direction.normalize();

                        // Calculate the target rotation angle in radians
                        float targetRadians = atan2(dyLength, dxLength);
                        //Converting to degrees
                        targetRotation = targetRadians * 180.0f / static_cast<float>(M_PI);

                        // This is meant to determine whether the enemy is on the left or right side. 
                        //Such an operation would allow to rotate the turret the shortest way possible
                        rotationDifference = targetRotation - currentRotation;
                        if (rotationDifference > 180.0f)
                        {
                            rotationDifference -= 360.0f;
                        }
                        else
                            if (rotationDifference < -180.0f)
                            {
                                rotationDifference += 360.0f;
                            }
                        currentRotation += rotationDifference * rotationSpeed;
                        // Set the turret's rotation
                        entity->getComponent<SpriteComponent>().setRotation(currentRotation);


                        // If the required time has passed and the turret is aligned, shoot
                        if (elapsedTime.count() >= shootDelayDuration && (rotationDifference) < 10) {
                            std::string enemyFileName = "assets/projectile" + std::to_string(turretColor) + ".png";
                            CreateProjectile(Vector2D(static_cast<float>(A0.x), static_cast<float>(A0.y)), direction, projectileRange, projectileSpeed, projectileDamage, enemyFileName.c_str(), &manager, *e, turretColor);
                            lastShotTime = std::chrono::steady_clock::now();
                            entity->getComponent<SpriteComponent>().Play("Shoot", shootDelayDuration);

                        }

                        //This is required so that the turret will only shoot the first enemy in range
                        //If the enemy is inside the range, it will shoot it, break the cycle, start the cycle again and shoot the first enemy
                        //And if the enemy is outside of range, the turret simply won`t shoot, thus moving onto the next enemy
                        break;
                    }
                }
                    else
                    {
                        // If the enemy is not in range, set the turret to idle
                        entity->getComponent<SpriteComponent>().Play("Idle");
                    }
                
            }
        }
        else {
            // If there are no enemies, set the turret to idle
            entity->getComponent<SpriteComponent>().Play("Idle");
        }
    }


    void update() override {
        //For rendering the base of the turret that is not animated
        destRect.x = static_cast<int>(entity->getComponent<TransformComponent>().position.x) - Game::camera.x;
        destRect.y = static_cast<int>(entity->getComponent<TransformComponent>().position.y) - Game::camera.y;

        destRect.w = entity->getComponent<TransformComponent>().width;
        destRect.h = entity->getComponent<TransformComponent>().height;
        //Calling shoot() on a regular basis
        entity->getComponent<TurretComponent>().shoot(entity->getManager());
    }
    void draw() override {
     Draw(texture, srcRect, destRect, SDL_FLIP_NONE, 0);
    }

};
