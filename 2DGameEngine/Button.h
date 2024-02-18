#pragma once
#include "Components.h"
//����: ����� ������, ���� ������� �������� �� ��������� �� �����������. ���� � ����� ������� ���� LabelComponent �������� �� ����� � ������������ �������. 
// �� ����� �� ��� ����: ��� �� ������� �������+��������� �����, ��� �� ������� �����
class Button :public Component {
public:
private:
    SDL_Rect src, dest, parent;
    SDL_Texture* button;
    bool drawbutton = false;
    int xPosTmp, yPosTmp;//��� ���������� ���������� ������� ������� ������


public:
    Button(int x, int y)
    {
        parent.x = x - Game::camera.x;
        parent.y = y - Game::camera.y;
        parent.h = parent.w = 128;
        src.x = src.y = 0;
        src.h = src.w = 64;
        dest.x = x; xPosTmp = x;
        dest.y = y; yPosTmp = y;
        dest.h = dest.w = 64;
        
    }

  
    void update() override {
        dest.x = xPosTmp; dest.y = yPosTmp;
        //���������� ������� ������
        dest.x -= Game::camera.x;
        dest.y -= Game::camera.y;
        parent.x = xPosTmp - Game::camera.x;
        parent.y = yPosTmp - Game::camera.y;
        click(parent);
    }
    bool click(SDL_Rect& testRect) {
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);

        SDL_Point mousePoint = { mouseX, mouseY };


        if (Game::event.type == SDL_MOUSEBUTTONDOWN) {
            if (drawbutton)drawbutton = !drawbutton;
            switch (Game::event.button.button) {
            case SDL_BUTTON_LEFT:
                if (SDL_PointInRect(&mousePoint, &testRect)) {
                    drawbutton = !drawbutton;
                    return drawbutton;
                }
                break;
            }


        }

    }

};
