#pragma once

#include <map>
#include <string>
#include "TextureManager.h"
#include "Vector2D.h"
#include "ECS.h"
#include "SDL_ttf.h"

class ProjectileManager
{
public:
	ProjectileManager();
	~ProjectileManager();

	//gameobjects

	static void CreateProjectile(Vector2D pos, Vector2D vel, int range, int speed, const char* id, Manager* man);


	

private:

	
};