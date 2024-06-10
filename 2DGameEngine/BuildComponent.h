#pragma once
#include "Components.h"

// Class that allows to build and rebuild turrets
class BuildComponent : public Component
{
private:
    //For texture rendering
    SDL_Rect parent;
    SDL_Rect src[4];
    SDL_Rect dest[4];
    SDL_Texture *texture[4];
    //Flag to indicate when to draw the build interface
    //BY default the buttons for build are not drawn
    bool drawbutton = false;
    //The position of the tile
    int xPosTmp, yPosTmp;
 

public:
    // Default constructor
    BuildComponent() = default;
    BuildComponent(int xPos, int Ypos)
    {
        //The position of tile
        parent.x = xPos - Game::camera.x;
        parent.y = Ypos - Game::camera.y;
        xPosTmp = xPos;
        yPosTmp = Ypos;
        parent.h = parent.w = 128;
        //Assigning the values for button rendering
        for (int i = 0; i < 4; i++) {
            src[i].x = src[i].y = 0;
            src[i].w = src[i].h = 64;
            dest[i].w = dest[i].h = 64;
            std::string fileName = "assets/button" + std::to_string(i+1) + ".png";
            texture[i] = LoadTexture(fileName.c_str());
        }
    }
   
    //The fucntion that would draw the buttons to build the turrets
    void click(SDL_Rect& Rect)
    {
        //VAriables for mouse state
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        SDL_Point mousePoint = { mouseX, mouseY };

        // if the left mouse button is pressed
        if (Game::event.type == SDL_MOUSEBUTTONDOWN && Game::pauseSwitch==true)
        {
            //if the buttons are already drawn, and the click is outside the tile
            //stop the drawing of buttons
            //if buttons are not drawn, just proceed ahead
            if (drawbutton)
                drawbutton = !drawbutton;
            switch (Game::event.button.button)
            {
            case SDL_BUTTON_LEFT:
                //if the click has occured on the desired tile, set draw button from false to true
                //That would be used in the next function
                //&Rect is the rectangle of the tile on which we can build turrets
                if (SDL_PointInRect(&mousePoint, &Rect))
                {
                    drawbutton = !drawbutton;
                }
                break;
            }
        }
    }


    //Render function
    void draw() override
    {
        click(parent);
        //If the flag is set true, draw the buttons
        if (drawbutton == true)
        {
            for (int i = 0; i < 4; i++) {
                Draw(texture[i], src[i], dest[i], SDL_FLIP_NONE, 0);

            }
        }
    }



    //Update function
    void update() override
    {
        // Buttons positions update
        dest[0].x = xPosTmp - 64;
        dest[0].y = yPosTmp - 64;
        dest[1].x = xPosTmp + 128;
        dest[1].y = yPosTmp - 64;
        dest[2].x = xPosTmp + 128;
        dest[2].y = yPosTmp + 128;
        dest[3].x = xPosTmp - 64;
        dest[3].y = yPosTmp + 128;

        for (int i = 0; i < 4; i++) {
            dest[i].x -= Game::camera.x;
            dest[i].y -= Game::camera.y;
        }
        parent.x = xPosTmp - Game::camera.x;
        parent.y = yPosTmp - Game::camera.y;

        // Variables for mouse position
        int mouseX, mouseY;
        Uint32 mousestate = SDL_GetMouseState(&mouseX, &mouseY);
        SDL_Point mousePoint = { mouseX, mouseY };

        // If there was a click and the buttons are drawn
        if (Game::event.type == SDL_MOUSEBUTTONDOWN && drawbutton == true)
        {
            switch (Game::event.button.button)
            {
            case SDL_BUTTON_LEFT:
                for (int i = 0; i < 4; i++) {
                    //variables for resourse files
                    std::string base = "assets/turret" + std::to_string(i + 1) + "_base.png";
                    std::string anims = "assets/turret" + std::to_string(i + 1) + "_anims.png";
                    //Checks if any of the buttons is clicked and build turret if done so
                    if (SDL_PointInRect(&mousePoint, &dest[i])) { Game::AddTurret(parent.x + Game::camera.x, parent.y + Game::camera.y, base.c_str(), anims.c_str(), i + 1); }
                    
                }
                break;
            }
        }
    }

   
   
};
