#pragma once
#include "Components.h"
#include "Game.h"
//Class for tha base 
class BaseComponent : public Component {
private:
	//FOr texture rendering
	SDL_Rect src;
	SDL_Rect dest;



public:
	//Default constructor
	BaseComponent() = default;
	void update() override {
		//CHecks the collision of each enemy against each base
		for (auto& e : entity->getManager().getGroup(Game::groupEnemies)) {
			for (auto b : entity->getManager().getGroup(Game::groupBases)) {
				//SDL function for collision detection
				if (SDL_HasIntersection(&e->getComponent<ColliderComponent>().collider, &b->getComponent<ColliderComponent>().collider))
				{
					//If the enemy has reached the base, destroy him and decrease Lives count
					e->destroy();
					Game::Lives--;
				}
			}
		}

		
	}

	void draw() override {
		//Draws the text tha displays the lives remaining
		//convert string to const char* using STD
		std::string file = "Lives remaining " + std::to_string(Game::Lives);
		const char* a = file.c_str();
		//For rendering
		src.x = src.y = 0; src.h = src.w = 256;
		dest.w = 312;
		dest.h = 64;
		dest.x =1920-dest.w;
		dest.y =0;
		//Rendering
		Draw(LoadFont("assets/font2.ttf", 8, a), src, dest, SDL_FLIP_NONE, 0);
	}
};