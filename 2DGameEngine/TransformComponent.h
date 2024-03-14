#pragma once
#include "Components.h"
#include "Vector2D.h"

class TransformComponent : public Component {

public:

	Vector2D position;
	Vector2D velocity;
	SDL_Point center;

	int height = 32;
	int width = 32;
	int scale = 1;
	int speed = 1;

	TransformComponent()
	{
		position.Zero();
	}

	TransformComponent(int scale)
	{
		//position.x = 400;
		//position.y = 320;
		this->scale = scale;
	}

	TransformComponent(float x, float y) 
	{
		position.x = x;
		position.y = y;
		
		
	}

	TransformComponent(float x, float y, int h, int w, int speed)
	{
		position.x = x;
		position.y = y;
		height = h;
		width = w;
		this->speed = speed;
	
	}
	
	void init() override {
		velocity.Zero();
	}
	void update() override
	{
		position.x += velocity.x * speed;
		position.y += velocity.y * speed;
		center.x = position.x + width ;
		center.y = position.y + height;
	}
	
};