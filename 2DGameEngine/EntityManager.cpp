#include "EntityManager.h"
#include "Components.h"
#include "Game.h"


EntityManager::EntityManager() 
{}

EntityManager::~EntityManager()
{}
//Creating a projectile
void EntityManager::CreateProjectile(Vector2D pos, Vector2D vel, int range, int speed, int dmg, const char *id, Manager* man, Entity& enemy, int type)
{
	//Creating using the manager
	auto& projectile(man->addEntity());
	//Passing the position size, and speed
	projectile.addComponent<TransformComponent>(pos.x, pos.y, 16, 16, speed);
	//Passing the path and whether is animated
	projectile.addComponent<SpriteComponent>(id, false);
	//Passing range, speed. velocity, damage, target, ane the color of the projectile
	projectile.addComponent<ProjectileComponent>(range, speed, vel, dmg, enemy, type);
	//Creating the collider
	projectile.addComponent<ColliderComponent>();
	//Adding to the group
	projectile.addGroup(Game::groupProjectiles);

}
//Creating an enemy
 void EntityManager::CreateEnemy(Vector2D pos, Vector2D vel, int speed,int health, const char* id, Manager* man, int type)
{
	 //Creating using the manager
	auto& enemy(man->addEntity());
	//Passing the position, size and speed
	enemy.addComponent<TransformComponent>(pos.x, pos.y, 32, 32, speed);
	//Passing the path and whether is animated
	enemy.addComponent<SpriteComponent>(id, false);
	//Passing the health, velocity, and color of the enemy
	enemy.addComponent<EnemyComponent>( health, vel, type);
	//Adding the collider
	//It is initialized in its init() function
	enemy.addComponent<ColliderComponent>();
	//Adding to the group
	enemy.addGroup(Game::groupEnemies);

}

