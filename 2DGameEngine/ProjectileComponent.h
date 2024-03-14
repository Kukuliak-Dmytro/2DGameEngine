#pragma once

#include "ECS.h"
#include "Components.h"
#include "Vector2D.h"
#include "Tools.h"

class ProjectileComponent : public Component
{
private:
	TransformComponent* transform;
	//Projectile data
	int damage = 0;
	int range = 0;
	int speed = 0;
	int distance = 0;
	//Projectile vector that holds the speed and direction
	Vector2D velocit;
	//A reference to the enemy at which the projectile was fired
	Entity& target;
public:
	ProjectileComponent(int rng, int sp, Vector2D vel, int dmg, Entity& enemy) : range(rng), speed(sp), velocit(vel), damage(dmg), target(enemy)
	{}
	~ProjectileComponent() = default;

	void init() override
	{
		//It is designed that the projectile would fly in a straight line
		//The velocity vector is the direction and speed of the projectile -is set here
		transform = &entity->getComponent<TransformComponent>();
		transform->velocity = velocit*speed;
	}

	void update() override
	{
		//each projectile has a range
		distance += speed;
		//and if the distance travelled is greater than range the projectile , it disappears
		if (distance > range)
		{
			std::cout << "Out of Range" << std::endl;
			entity->destroy();
		}

		setAim();
	
	}
	//Setters and getters to access private data
	int getDmg() {return damage;}
	void setDmg(int x) { damage = x; }

	//Function that makes the projectiles chase the enemy some time after beign fired
	//It is called in update() function
	void setAim() {		
		//If the targeted enemy is still alive
		//Possible to add a mechanism so that the projectile would find a target after its death 
		if (target.isActive() == true) {
			//Let the projectile fly some distance without chasing the enemy to look more natural
			//But not too much or too little
			if (distance > 50) {
				//Calculate the vertical and horizontal distances
				float dxLength = tools::dx(entity->getComponent<ColliderComponent>().collider, target.getComponent<ColliderComponent>().collider);
				float dyLength = tools::dy(entity->getComponent<ColliderComponent>().collider, target.getComponent<ColliderComponent>().collider);
				//Define a vector with parameters of direction
				Vector2D test(-dxLength, -dyLength);
				//Call a normalize function that would set the modulus of vector(length a.k.a. speed in our case) to "speed" variable
				test.normalize(speed);
				//And change the diraction of projectile to the vector pointing at the enemy
				transform->velocity = test;
			}
		}
		
	}




};