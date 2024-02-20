#pragma once
#include "ECS.h"
#include "Components.h"
#include "Vector2D.h"


class EnemyComponent : public Component
{
public:
	EnemyComponent(){}
	EnemyComponent(int hp, Vector2D vel) :velocit(vel),health(hp) {}
	~EnemyComponent() {}
	void init() override
	{
		transform = &entity->getComponent<TransformComponent>();
		transform->velocity = velocit;
	}

	void update() override
	{
		if (health < 0) {
			std::cout << "Health reached zero";
			entity->destroy();
		}

		
	}

private:

	TransformComponent* transform;
	int health=100;
	
	Vector2D velocit;
	int coins;



};