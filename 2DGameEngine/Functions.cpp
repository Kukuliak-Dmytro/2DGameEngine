#include "Components.h"
#include "Functions.h"
#include <cmath>
#include <fstream>
#include <string>

// ������� ��� ������������ �������� � �����
SDL_Texture* LoadTexture(const char* texture) {
    SDL_Surface* tempSurface = IMG_Load(texture); // ������������ ���������� � ����� � ��������
    SDL_Texture* tex = SDL_CreateTextureFromSurface(Game::renderer, tempSurface); // ��������� �������� � �������
    SDL_FreeSurface(tempSurface); // ��������� �������

    return tex; // ���������� �������� ��������
}

// ������� ��� ������������ ������ � ��������� �������� ��������
SDL_Texture* LoadFont(std::string fontPath, int fontSize, const char* fontText) {
    TTF_Font* font = TTF_OpenFont(fontPath.c_str(), fontSize); // ³������� ������
    SDL_Color color = { 128, 128, 128, 255 }; // ������������ ������� ������ (����)
    SDL_Surface* tempSurface = TTF_RenderText_Blended(font, fontText, color); // ��������� ������ � ��������
    SDL_Texture* tex = SDL_CreateTextureFromSurface(Game::renderer, tempSurface); // ��������� �������� � �������
    SDL_FreeSurface(tempSurface); // ��������� �������

    return tex; // ���������� �������� ��������
}

// ������� ��� ��������� �������� � ��������� ��������
void Draw(SDL_Texture* tex, SDL_Rect src, SDL_Rect dest, SDL_RendererFlip flip, int angle)
{
    // ������ SDL_RenderCopyEx ��� ��������� �������� � ��������� �������� � �����������
    SDL_RenderCopyEx(Game::renderer, tex, &src, &dest, angle, NULL, flip);
}

// ������� ��� ������������ ����� � �����
void LoadMap(std::string path, int sizeX, int sizeY)
{
    char c; // ����� ��� ���������� ������� � �����
    std::fstream mapFile; // ��'��� ��� ������ � ������
    mapFile.open(path); // ³������� ����� � ������

    int srcX; // ���������� X ��� ������� ��������
    int srcY; // ���������� Y ��� ������� ��������

    // ������ �� ��� ���� �� Y �� X �����������
    for (int y = 0; y < sizeY; y++) {
        for (int x = 0; x < sizeX; x++) {
            mapFile.get(c); // ���������� ������� � �����
            srcY = atoi(&c) * 128; // ���������� ���������� Y ������� ��������
            mapFile.get(c); // ���������� ���������� �������
            srcX = atoi(&c) * 128; // ���������� ���������� X ������� ��������
            if (atoi(&c) == 2) {
                Game::AddTile(srcX, srcY, x * 128, y * 128, true); // ��������� ������ � ���糺�
            }
            else {
                Game::AddTile(srcX, srcY, x * 128, y * 128, false); // ��������� ������ ��� ���糿
            }
            mapFile.ignore(); // ����������� ���������� ������� (���������, ������ ��� ���� ��)
        }
    }
    mapFile.close(); // �������� ����� ���� ����������
}

// ������� ��� ���������� ������������� ������ �� �������� ���� ������������
float dx(const SDL_Rect& rect1, const SDL_Rect& rect2) {
    if (&rect2 == nullptr) {
        return 0.0f; // �������� �� �������� ��������
    }
    // ���������� ������ ��������� X ������ ���� ������������
    return (rect1.x + (rect1.w / static_cast<float>(2))) - (rect2.x + (rect2.w / static_cast<float>(2)));
}

// ������� ��� ���������� ����������� ������ �� �������� ���� ������������
float dy(const SDL_Rect& rect1, const SDL_Rect& rect2) {
    if (&rect2 == nullptr) {
        return 0.0f; // �������� �� �������� ��������
    }
    // ���������� ������ ��������� Y ������ ���� ������������
    return (rect1.y + (rect1.h / static_cast<float>(2))) - (rect2.y + (rect2.h / static_cast<float>(2)));
}

// ������� ��� ���������� ������ �� �������� ���� ������������
float distance(SDL_Rect rect1, SDL_Rect rect2)
{
    // ���������� ��������� ������ ������� ������������
    float x1 = static_cast<float>(rect1.x) + static_cast<float>(rect1.w) / 2;
    float y1 = static_cast<float>(rect1.y) + static_cast<float>(rect1.h) / 2;

    // ���������� ��������� ������ ������� ������������
    float x2 = static_cast<float>(rect2.x) + static_cast<float>(rect2.w) / 2;
    float y2 = static_cast<float>(rect2.y) + static_cast<float>(rect2.h) / 2;

    // ���������� ������ �� ��������� ������� ϳ������
    float delta = static_cast<float>(sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2)));

    return delta; // ���������� ��������� ������
}

// ������� ��� ��������� �������
void CreateProjectile(Vector2D pos, Vector2D vel, int range, int speed, int dmg, const char* id, Manager* man, Entity& enemy, int type)
{
    // ��������� � ������������� ���������
    auto& projectile(man->addEntity());
    // �������� �������, ������ �� ��������
    projectile.addComponent<TransformComponent>(pos.x, pos.y, 16, 16, speed);
    // �������� ����� �� �������� � �������, �� ��������� ������
    projectile.addComponent<SpriteComponent>(id, false);
    // �������� ��������, ��������, ������� ��������, �����, ��� �� ���� �������
    projectile.addComponent<ProjectileComponent>(range, speed, vel, dmg, enemy, type);
    // ��������� ���������
    projectile.addComponent<ColliderComponent>();
    // ��������� �� ����� �������
    projectile.addGroup(Game::groupProjectiles);
}

// ������� ��� ��������� ������
void CreateEnemy(Vector2D pos, Vector2D vel, int speed, int health, const char* id, Manager* man, int type)
{
    // ��������� � ������������� ���������
    auto& enemy(man->addEntity());
    // �������� �������, ������ �� ��������
    enemy.addComponent<TransformComponent>(pos.x, pos.y, 32, 32, speed);
    // �������� ����� �� �������� � �������, �� ��������� ������
    enemy.addComponent<SpriteComponent>(id, false);
    // �������� ������'�, ������� �������� �� ���� ������
    enemy.addComponent<EnemyComponent>(health, vel, type);
    // ��������� ���������
    enemy.addComponent<ColliderComponent>();
    // ��������� �� ����� ������
    enemy.addGroup(Game::groupEnemies);
}

// ������� ��� ���������� �����
void playSound(const char* path) {
    Mix_Chunk* sound = Mix_LoadWAV(path); // ������������ ����� � �����
    if (sound == NULL) {
        std::cout << "Failed to load sound effect! SDL_mixer Error: " << Mix_GetError(); // ��������� �������, ���� ���� �� ������� �����������
    }
    Mix_PlayChannel(-1, sound, 0); // ³��������� �����
    // Mix_FreeChunk(sound); // ��������� ����� (���� �������)
}
