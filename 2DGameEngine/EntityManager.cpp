#include "EntityManager.h"
#include "Components.h"
#include "Game.h"


EntityManager::EntityManager() 
{}

EntityManager::~EntityManager()
{}

void EntityManager::CreateProjectile(Vector2D pos, Vector2D vel, int range, int speed, const char *id, Manager* man)
{
	auto& projectile(man->addEntity());
	projectile.addComponent<TransformComponent>(pos.x, pos.y, 32, 32, 1);
	projectile.addComponent<SpriteComponent>(id, false);
	projectile.addComponent<ProjectileComponent>(range, speed, vel);
	projectile.addComponent<ColliderComponent>("projectile");
	projectile.addGroup(Game::groupProjectiles);
}

 void EntityManager::CreateEnemy(Vector2D pos, Vector2D vel, const char* id, Manager* man)
{
	auto& enemy(man->addEntity());
	enemy.addComponent<TransformComponent>(pos.x, pos.y, 32, 32, 1);
	enemy.addComponent<SpriteComponent>(id, false);
	enemy.addComponent<EnemyComponent>( 100, vel);
	enemy.addComponent<ColliderComponent>("projectile");
	enemy.addGroup(Game::groupEnemies);
}

