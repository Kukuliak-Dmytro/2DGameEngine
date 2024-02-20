#include "ProjectileManager.h"
#include "Components.h"
#include "Game.h"

ProjectileManager::ProjectileManager() 
{}

ProjectileManager::~ProjectileManager()
{}

void ProjectileManager::CreateProjectile(Vector2D pos, Vector2D vel, int range, int speed, const char *id, Manager* man)
{
	auto& projectile(man->addEntity());
	projectile.addComponent<TransformComponent>(pos.x, pos.y, 32, 32, 1);
	projectile.addComponent<SpriteComponent>(id, false);
	projectile.addComponent<ProjectileComponent>(range, speed, vel);
	projectile.addComponent<ColliderComponent>("projectile");
	projectile.addGroup(Game::groupProjectiles);
}


