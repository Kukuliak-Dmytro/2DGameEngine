#pragma once
#include "Components.h"
#include "Game.h"

class TransformComponent : public Component {

public:	
	Vector2D position;
	Vector2D velocity;


	int height = 32;
	int width = 32;

	int speed = 1;

	TransformComponent()
	{
		position.Zero();
	}

	

	TransformComponent(float x, float y) 
	{
		position.x = x;
		position.y = y;

		
		
	}
	TransformComponent(float x, float y, int h, int w)
	{
		position.x = x;
		position.y = y;

		height = h;
		width = w;

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
		
	}
	
};