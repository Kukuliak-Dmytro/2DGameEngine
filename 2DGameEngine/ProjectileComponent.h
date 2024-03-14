#pragma once

#include "ECS.h"
#include "Components.h"
#include "Vector2D.h"

class ProjectileComponent : public Component
{
public:
	ProjectileComponent(int rng, int sp, Vector2D vel, int dmg) : range(rng), speed(sp), velocit(vel), damage(dmg)
	{}
	~ProjectileComponent()
	{}

	void init() override
	{
		transform = &entity->getComponent<TransformComponent>();
		transform->velocity = velocit*speed;
	}

	void update() override
	{
		distance += speed;

		if (distance > range)
		{
		//	std::cout << "Out of Range" << std::endl;
			entity->destroy();
		}
	
	}

	int getDmg() {
		return damage;
	}
	void setDmg(int x) {
		 damage=x;
	}

private:

	TransformComponent* transform;
	int damage = 0;
	int range = 0;
	int speed = 0;
	int distance = 0;
	Vector2D velocit;


};