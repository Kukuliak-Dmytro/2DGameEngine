#pragma once

#include <map>
#include <string>
#include "TextureManager.h"
#include "Vector2D.h"
#include "ECS.h"
#include "SDL_ttf.h"

class EntityManager
{
public:
	EntityManager();
	~EntityManager();

	//gameobjects

	static void CreateProjectile(Vector2D pos, Vector2D vel, int range, int speed,int dmg, const char* id, Manager* man, Entity& enrmy);
	static void CreateEnemy(Vector2D pos, Vector2D vel, const char* id, Manager* man);


	

private:

	
};