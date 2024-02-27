#pragma once
#include "Components.h"

class BuildComponent : public Component
{
private:
    SDL_Rect src1, dest1, parent;
    SDL_Rect src2, dest2;
    SDL_Rect src3, dest3;
    SDL_Rect src4, dest4;
    SDL_Texture *button1, *button2, *button3, *button4;
    bool 
        drawbutton = false;
    int xPosTmp, yPosTmp;//для динамічного визначення позиції рендеру кнопок


public:
    BuildComponent() = default;
    BuildComponent(int xPos, int Ypos)
    {
        parent.x = xPos-Game::camera.x;
        parent.y = Ypos-Game::camera.y;
        parent.h = parent.w = 128;
        src1.x = src1.y = src2.x = src2.y = src3.x = src3.y = src4.x = src4.x = 0;
        src1.w = src1.h = src2.w = src2.h = src3.w = src3.h = src4.w = src4.h = 64;
       
        xPosTmp = xPos;        
        yPosTmp = Ypos;
        dest1.h = dest1.w = 64;
        dest2.h = dest2.w = 64;
        dest3.h = dest3.w = 64;
        dest4.h = dest4.w = 64;
        button1 = TextureManager::LoadTexture("assets/button1.png");
        button2 = TextureManager::LoadTexture("assets/button2.png");
        button3 = TextureManager::LoadTexture("assets/button3.png");
        button4 = TextureManager::LoadTexture("assets/button4.png");
        
    }
        
    void draw() override {
        click(parent);
        if (drawbutton == true) {
             
            TextureManager::Draw(button1, src1, dest1, SDL_FLIP_NONE,0);
            TextureManager::Draw(button2, src2, dest2, SDL_FLIP_NONE,0);
            TextureManager::Draw(button3, src3, dest3, SDL_FLIP_NONE,0);
            TextureManager::Draw(button4, src4, dest4, SDL_FLIP_NONE,0);

               
                

            
        }
    }
    void update() override {
        dest1.x = xPosTmp-64;           dest1.y = yPosTmp-64;
        dest2.x = xPosTmp+128;           dest2.y = yPosTmp-64;
        dest3.x = xPosTmp +128;          dest3.y = yPosTmp + 128;
        dest4.x = xPosTmp -64;            dest4.y = yPosTmp+128;
        //врахування позиції камери
        dest1.x -= Game::camera.x;
        dest1.y -= Game::camera.y;
        dest2.x -= Game::camera.x;
        dest2.y -= Game::camera.y;
        dest3.x -= Game::camera.x;
        dest3.y -= Game::camera.y;
        dest4.x -= Game::camera.x;
        dest4.y -= Game::camera.y;
        parent.x = xPosTmp - Game::camera.x;
        parent.y = yPosTmp - Game::camera.y;
        ////////////////////////
        int mouseX, mouseY;
        Uint32 mousestate = SDL_GetMouseState(&mouseX, &mouseY);
        SDL_Point mousePoint = { mouseX, mouseY };
        if (Game::event.type == SDL_MOUSEBUTTONDOWN&&drawbutton==true) {

            switch (Game::event.button.button) {
            case SDL_BUTTON_LEFT:
                if (SDL_PointInRect(&mousePoint, &dest1)) { Game::AddTurret(parent.x + Game::camera.x, parent.y + Game::camera.y); }
                if (SDL_PointInRect(&mousePoint, &dest2)) { Game::AddTurret(parent.x + Game::camera.x, parent.y + Game::camera.y); }
                if (SDL_PointInRect(&mousePoint, &dest3)) { Game::AddTurret(parent.x + Game::camera.x, parent.y + Game::camera.y); }
                if (SDL_PointInRect(&mousePoint, &dest4)) { Game::AddTurret(parent.x + Game::camera.x, parent.y + Game::camera.y); }

                break;
            }


        }

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
