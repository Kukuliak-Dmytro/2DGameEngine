#include "EntityManager.h"
#include "Components.h"
#include "Game.h"


EntityManager::EntityManager() 
{}

EntityManager::~EntityManager()
{}

void EntityManager::CreateProjectile(Vector2D pos, Vector2D vel, int range, int speed, int dmg, const char *id, Manager* man)
{
	auto& projectile(man->addEntity());
	projectile.addComponent<TransformComponent>(pos.x, pos.y, 16, 16, speed);
	projectile.addComponent<SpriteComponent>(id, false);
	projectile.addComponent<ProjectileComponent>(range, speed, vel, dmg);
	projectile.addComponent<ColliderComponent>("projectile");
	projectile.addGroup(Game::groupProjectiles);
	//std::cout << "Projectile created";
}

 void EntityManager::CreateEnemy(Vector2D pos, Vector2D vel, const char* id, Manager* man)
{
	auto& enemy(man->addEntity());
	enemy.addComponent<TransformComponent>(pos.x, pos.y, 32, 32, 5);
	enemy.addComponent<SpriteComponent>(id, false);
	enemy.addComponent<EnemyComponent>( 100, vel);
	enemy.addComponent<ColliderComponent>("projectile");
	enemy.addGroup(Game::groupEnemies);
	std::cout << "Enemy created";
}

