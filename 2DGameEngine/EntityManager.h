#pragma once

#include <map>
#include <string>
#include "TextureManager.h"
#include "Vector2D.h"
#include "ECS.h"
#include "SDL_ttf.h"
//Class that spawns enemies and projectiles
class EntityManager
{
public:
	//Constructor&Destructor
	EntityManager();
	~EntityManager();

	//They are static so that we won`t need to create an object
	//Realistically, these methods might be declared as simple functions, there is no need for a class
	static void CreateProjectile(Vector2D pos, Vector2D vel, int range, int speed,int dmg, const char* id, Manager* man, Entity& enemy, int type);
	static void CreateEnemy(Vector2D pos, Vector2D vel,int speed,int health, const char* id, Manager* man, int type);

	
};