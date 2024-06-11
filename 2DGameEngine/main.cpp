#include "Game.h"
Game* game = nullptr;

int main(int argc, char* argv[]) {
    // ������������ ������� ����� � ������� (FPS)
    const int FPS = 60;
    // ���������� �������� �� ������� � ����������
    const int frameDelay = 1000 / FPS;

    Uint32 frameStart; // ����� ��� ��������� ���� ������� �����
    int frameTime; // ����� ��� ��������� ����, ���� ���������� ��� ������� �����

    game = new Game(); // ��������� ����� ��'��� ���

    // ���������� ��� � ����� ������� 1920x1080, ������������ � ����� ������
    game->init("Tower Defense", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1920, 1080, false);

    // �������� ������� ����
    while (game->running()) {
        frameStart = SDL_GetTicks(); // �������� ��� ������� �����

        game->handleEvents(); // ������� ���� (���������, ���������� �����, ��� ���� ����)
        game->update(); // ��������� ����� ��� (����� ���)
        game->render(); // ³���������� ��� �� �����

        frameTime = SDL_GetTicks() - frameStart; // ���������� ���, ���� ���������� ��� ������� �����

        // ���� ��� ������� ����� ������ �� ����������� �������� �� �������, �������� ��������
        if (frameDelay > frameTime) {
            SDL_Delay(frameDelay - frameTime); // ��������, ��� ����������� ������� ������� �����
        }
    }

    game->clean(); // �������� ������� ���
    delete game; // ��������� ��'���� ���
    return 0;
}
