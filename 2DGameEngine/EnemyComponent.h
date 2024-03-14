#pragma once
#include "ECS.h"
#include "Components.h"
#include "Vector2D.h"



class EnemyComponent : public Component
{
public:
	EnemyComponent(){}
	EnemyComponent(int hp, Vector2D vel) :velocit(vel),health(hp) 
	{
	
	}
	~EnemyComponent() {}
	void init() override
	{
		transform = &entity->getComponent<TransformComponent>();
		transform->velocity = velocit;
	}

	void update() override
	{
		if (health <= 0) {
			std::cout << "Health reached zero";
			entity->destroy();
		}
		
		entity->getComponent<EnemyComponent>().checkCollision(entity->getManager());
		
	}
	void draw() override{
		src.x = src.y = 0; src.h = src.w = 10;
		dest.x = transform->position.x -Game::camera.x;
		dest.y = transform->position.y - 10-Game::camera.y;
		dest.w = 32;
		dest.h = 4;
		TextureManager::Draw(TextureManager::LoadTexture("assets/health_bar_back.png"), src, dest, SDL_FLIP_NONE, 0);		
		
		src.x=0; src.y = (health/10)-1; src.h = 1; src.w = health/10;
		dest.x = transform->position.x - Game::camera.x+2;
		dest.y = transform->position.y - 10 - Game::camera.y+1;
		dest.w = 28* src.w/10;
		dest.h = 2;
		TextureManager::Draw(TextureManager::LoadTexture("assets/health_bar.png"), src, dest, SDL_FLIP_NONE, 0);
	}

	void checkCollision(Manager& manager) {
		for (auto& e : manager.getGroup(Game::groupEnemies)) {
			for (auto& p : manager.getGroup(Game::groupProjectiles))
				if (SDL_HasIntersection(&e->getComponent<ColliderComponent>().collider, &p->getComponent<ColliderComponent>().collider)) {
					health = health - p->getComponent<ProjectileComponent>().getDmg();
					p->getComponent<ProjectileComponent>().setDmg(0);
					std::cout << health << std::endl;
			}
		}
	}
private:

	TransformComponent* transform;
	Vector2D velocit;
	int health=100;
	int coins;
	SDL_Rect src;
	SDL_Rect dest;


};