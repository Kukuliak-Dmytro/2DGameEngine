#pragma once
#include "Components.h"
#include "ProjectileComponent.h"




class EnemyComponent : public Component
{
private:
	//A pointer to the <TransformComponent>
	TransformComponent* transform;

	Vector2D Direction;
	int health;
	SDL_Rect src;
	SDL_Rect dest;
	

public:
	int enemyColor = 1;
	EnemyComponent(){}
	EnemyComponent(int hp, Vector2D vel, int color) :Direction(vel),health(hp), enemyColor(color)
	{
	
	}
	~EnemyComponent() {}
	void init() override
	{
		//The enemy would move linearly according to the velococity vector
		transform = &entity->getComponent<TransformComponent>();
		transform->velocity = Direction;
	}
	//The function draws a health bar over the opponent
	//Basically draw a black bar first, and then a colored one based on health
	//The source rectangle is dinamically changed so that the bar changes as well
	void draw() override{
		src.x = src.y = 0; src.h = src.w = 10;
		dest.x = transform->position.x -Game::camera.x;
		dest.y = transform->position.y - 10-Game::camera.y;
		dest.w = 32;
		dest.h = 4;
		Draw(LoadTexture("assets/health_bar_back.png"), src, dest, SDL_FLIP_NONE, 0);		
		
		src.x=0; src.y = (health/10)-1; src.h = 1; src.w = health/10;
		dest.x = transform->position.x - Game::camera.x+2;
		dest.y = transform->position.y - 10 - Game::camera.y+1;
		dest.w = 28* src.w/10;
		dest.h = 2;
		Draw(LoadTexture("assets/health_bar.png"), src, dest, SDL_FLIP_NONE, 0);
	}

	//Function that checks each projectile against each enemy
	void checkCollision(Manager& manager) {
		for (auto& e : manager.getGroup(Game::groupEnemies)) {
			for (auto& p : manager.getGroup(Game::groupProjectiles)) {
			
				//SDL_HasIntersection is a bulit-in function used for defining collision between two "SDL_Rect" objects
				if (SDL_HasIntersection(&e->getComponent<ColliderComponent>().collider, &p->getComponent<ColliderComponent>().collider)==true && p->getComponent<ProjectileComponent>().projectileColor == e->getComponent<EnemyComponent>().enemyColor) {
					//Subtract the damage inflicted
					e->getComponent<EnemyComponent>().health = e->getComponent<EnemyComponent>().health - p->getComponent<ProjectileComponent>().getDmg();
					//It is required so that the projectile would only hit ONE enemy and disappear, otherwise it would hit the closese ones as well
					//Remove this for creating projectiles with AOE damage
					p->getComponent<ProjectileComponent>().setDmg(0);
					//Destroy the projectle on impact
					//Remove for penetrating bullets
					p->destroy();
					//std::cout << health << std::endl;
					
				}
			}
			
		}
	}
	void update() override

	{
		if (entity->getComponent<TransformComponent>().position.x > 1920 + Game::camera.w || entity->getComponent<TransformComponent>().position.x < 0 - Game::camera.w || entity->getComponent<TransformComponent>().position.y > 1080 + Game::camera.h || entity->getComponent<TransformComponent>().position.y <0 - Game::camera.h)entity->destroy();
		//Destroying enemy if health reached zero
		if (health <= 0) {
			//std::cout << "Health reached zero";
			entity->destroy();
			Game::DefeatedEnemies++;
			
		}
		//Looping checking collisions
		entity->getComponent<EnemyComponent>().checkCollision(entity->getManager());

	}



};