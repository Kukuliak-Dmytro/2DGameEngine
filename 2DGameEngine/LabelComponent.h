#pragma once
#include "Components.h"

class LabelComponent : public Component
{
private:
    SDL_Rect src, dest, parent;
    SDL_Texture* button;
    bool drawbutton = false;
    int xPosTmp, yPosTmp;//для динамічного визначення позиції рендеру кнопок


public:
    LabelComponent(int x, int y)
    {
        parent.x = x-Game::camera.x;
        parent.y = y-Game::camera.y;
        parent.h = parent.w = 128;
        src.x = src.y = 0;
        src.h = src.w = 64;
        dest.x = x; xPosTmp = x;
        dest.y = y; yPosTmp = y;
        dest.h = dest.w = 64;
        button = TextureManager::LoadTexture("assets/button.png");
        if (!button) {
            std::cerr << "Failed to load button texture!" << std::endl;
        }
    }

    void draw() override {
        click(parent);
        if(drawbutton==true)TextureManager::Draw(button, src, dest, SDL_FLIP_NONE);

    }
    void update() override {
        dest.x = xPosTmp; dest.y = yPosTmp;
        //врахування позиції камери
        dest.x -= Game::camera.x;
        dest.y -= Game::camera.y;
        parent.x = xPosTmp - Game::camera.x;
        parent.y = yPosTmp - Game::camera.y;
     }
    void click(SDL_Rect& testRect) {
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);

        SDL_Point mousePoint = { mouseX, mouseY };
        
        
             if (Game::event.type == SDL_MOUSEBUTTONDOWN) {
                 if (drawbutton)drawbutton = !drawbutton;
                switch (Game::event.button.button) {
                case SDL_BUTTON_LEFT:
                    if (SDL_PointInRect(&mousePoint, &testRect)) {
                        drawbutton = !drawbutton;
                    }
                    break;
                }
            
            
        }

    }

};

   /*
   if (Game::event.type == SDL_MOUSEBUTTONDOWN) {
        switch (Game::event.button.button) {
        case SDL_BUTTON_LEFT:
            // Handle left mouse button click
            break;
        case SDL_BUTTON_MIDDLE:
            // Handle middle mouse button click
            break;
        case SDL_BUTTON_RIGHT:
            // Handle right mouse button click
            break;
            // Add more cases as needed for other mouse buttons
        default:
            // Handle other cases or do nothing
            break;
        }
    }



    ////////////////////////////
                if (mouseState & SDL_BUTTON(SDL_BUTTON_LEFT)) {
                drawbutton = !drawbutton;
            }
    */
