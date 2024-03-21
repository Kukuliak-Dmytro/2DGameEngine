#pragma once

#include "Components.h" 
#include "SDL.h"		
#include "TextureManager.h"
#include "Animation.h"
#include <map>
#include <chrono>

class SpriteComponent : public Component{
private:
	TransformComponent *transform;
	SDL_Texture* texture;
	SDL_Rect srcRect, destRect;
	int rotation = 90;
	bool animated = false;
	int frames = 0;
	int speed = 0;
	bool shootFlag = false;
	//Flag that switches between animations
	//This is a bad approach, since I need to add an array and every time to skip through each frame.
	//But I have only 2 frames due to the simplicity of the game. So, this slack would pass

	std::chrono::steady_clock::time_point lastShotTime;

public:
	int animIndex = 0;
	std::map<const char*, Animation> animations;

	SDL_RendererFlip spriteFlip = SDL_FLIP_NONE;

	SpriteComponent() = default;
	SpriteComponent(const char* path) 
	{
		setTex(path);
	}
	SpriteComponent(const char* path, bool isAnimated)
	{
		animated = isAnimated;
		Animation idle = Animation(0, 1);
		Animation shoot = Animation(1, 2);
		Animation portal = Animation(2, 2);
		//Also idle animation is a bad approach. Again, because the game is simple, I didn`t add an idle animation
		//Instead, I just turn off the animation at idle animation
		
		
		animations.emplace("Idle", idle);
		animations.emplace("Shoot", shoot);
		animations.emplace("Portal", portal);

		Play("Idle");
		setTex(path);
	}

	
	virtual ~SpriteComponent() 
	{
		SDL_DestroyTexture(texture);
	}

	void setTex(const char* path)
	{
		texture = TextureManager::LoadTexture(path);
	}
	void init() override 
	{
		transform = &entity->getComponent<TransformComponent>();

		srcRect.x = srcRect.y = 0;
		srcRect.w = transform->width;
		srcRect.h = transform->height;
	}


	void update() override
	{
		if(animated)
		{
			//The current point of time
			auto currentTime = std::chrono::steady_clock::now();
			//I have no idea how it works, but the cicle works every "speed" milliseconds
			std::chrono::duration<double, std::milli> elapsedTime = currentTime - lastShotTime;
			if (elapsedTime.count() >= speed) {
				//Flip through frames, the code simplicity just because I have 2 frames only
				//A better approach would be to overload ++ operator and allow "frames" number of values, iterate each time,
				//and if the value exceeds "frames", to start at zero, or at first frame
				//Anyway, this game doesn`t require that difficult operations, for now
				srcRect.x = srcRect.w * !shootFlag*(frames-1);
				lastShotTime = std::chrono::steady_clock::now();
				shootFlag = !shootFlag;
			}
		}
		

		destRect.x =static_cast<int>(transform->position.x)-Game::camera.x;
		destRect.y =static_cast<int>(transform->position.y)-Game::camera.y;

		destRect.w = transform->width * transform->scale;
		destRect.h = transform->height * transform->scale;
	}
	void setRotation(float angle) { rotation = angle-270; }

	SDL_Rect& getRect() { return destRect; }

	void draw() override
	{
		TextureManager::Draw(texture, srcRect, destRect,spriteFlip, rotation);
		
	}

	void Play(const char* animName)	
		//If the function is called without speed parameter, there would be no animation
	{
		animated = false;
		frames = animations[animName].frames;
		animIndex = animations[animName].index;
		
	}

	void Play(const char* animName, int sp)
	{
		//If the function is called with speed variable, the sprite would be animated
		animated = true;
		frames = animations[animName].frames;
		animIndex = animations[animName].index;
		//Divide by two because the barrel of the gun needs time to move back and forth
		speed = sp/2;
	}


};