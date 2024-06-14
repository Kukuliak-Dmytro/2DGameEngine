#include "Components.h"
#include "Functions.h"
#include <cmath>
#include <fstream>
#include <string>

// Функція для завантаження текстури з файлу
SDL_Texture* LoadTexture(const char* texture) {
    SDL_Surface* tempSurface = IMG_Load(texture); // Завантаження зображення з файлу у поверхню
    SDL_Texture* tex = SDL_CreateTextureFromSurface(Game::renderer, tempSurface); // Створення текстури з поверхні
    SDL_FreeSurface(tempSurface); // Звільнення поверхні

    return tex; // Повернення створеної текстури
}

// Функція для завантаження шрифту і створення текстової текстури
SDL_Texture* LoadFont(std::string fontPath, int fontSize, const char* fontText) {
    TTF_Font* font = TTF_OpenFont(fontPath.c_str(), fontSize); // Відкриття шрифту
    SDL_Color color = { 128, 128, 128, 255 }; // Встановлення кольору тексту (сірий)
    SDL_Surface* tempSurface = TTF_RenderText_Blended(font, fontText, color); // Рендеринг тексту у поверхню
    SDL_Texture* tex = SDL_CreateTextureFromSurface(Game::renderer, tempSurface); // Створення текстури з поверхні
    SDL_FreeSurface(tempSurface); // Звільнення поверхні

    return tex; // Повернення створеної текстури
}

// Функція для малювання текстури з можливістю повороту
void Draw(SDL_Texture* tex, SDL_Rect src, SDL_Rect dest, SDL_RendererFlip flip, int angle)
{
    // Виклик SDL_RenderCopyEx для малювання текстури з можливістю повороту і відображення
    SDL_RenderCopyEx(Game::renderer, tex, &src, &dest, angle, NULL, flip);
}

// Функція для завантаження карти з файлу
void LoadMap(std::string path, int sizeX, int sizeY)
{
    char c; // Змінна для зчитування символів з файлу
    std::fstream mapFile; // Об'єкт для роботи з файлом
    mapFile.open(path); // Відкриття файлу з картою

    int srcX; // Координата X для джерела текстури
    int srcY; // Координата Y для джерела текстури

    // Прохід по всій карті по Y та X координатам
    for (int y = 0; y < sizeY; y++) {
        for (int x = 0; x < sizeX; x++) {
            mapFile.get(c); // Зчитування символу з файлу
            srcY = atoi(&c) * 128; // Обчислення координати Y джерела текстури
            mapFile.get(c); // Зчитування наступного символу
            srcX = atoi(&c) * 128; // Обчислення координати X джерела текстури
            if (atoi(&c) == 2) {
                Game::AddTile(srcX, srcY, x * 128, y * 128, true); // Додавання плитки з колізією
            }
            else {
                Game::AddTile(srcX, srcY, x * 128, y * 128, false); // Додавання плитки без колізії
            }
            mapFile.ignore(); // Ігнорування наступного символу (наприклад, пробілу або нової лінії)
        }
    }
    mapFile.close(); // Закриття файлу після завершення
}

// Функція для обчислення горизонтальної відстані між центрами двох прямокутників
float dx(const SDL_Rect& rect1, const SDL_Rect& rect2) {
    if (&rect2 == nullptr) {
        return 0.0f; // Перевірка на нульовий вказівник
    }
    // Обчислення різниці координат X центрів двох прямокутників
    return (rect1.x + (rect1.w / static_cast<float>(2))) - (rect2.x + (rect2.w / static_cast<float>(2)));
}

// Функція для обчислення вертикальної відстані між центрами двох прямокутників
float dy(const SDL_Rect& rect1, const SDL_Rect& rect2) {
    if (&rect2 == nullptr) {
        return 0.0f; // Перевірка на нульовий вказівник
    }
    // Обчислення різниці координат Y центрів двох прямокутників
    return (rect1.y + (rect1.h / static_cast<float>(2))) - (rect2.y + (rect2.h / static_cast<float>(2)));
}

// Функція для обчислення відстані між центрами двох прямокутників
float distance(SDL_Rect rect1, SDL_Rect rect2)
{
    // Обчислення координат центру першого прямокутника
    float x1 = static_cast<float>(rect1.x) + static_cast<float>(rect1.w) / 2;
    float y1 = static_cast<float>(rect1.y) + static_cast<float>(rect1.h) / 2;

    // Обчислення координат центру другого прямокутника
    float x2 = static_cast<float>(rect2.x) + static_cast<float>(rect2.w) / 2;
    float y2 = static_cast<float>(rect2.y) + static_cast<float>(rect2.h) / 2;

    // Обчислення відстані за допомогою теореми Піфагора
    float delta = static_cast<float>(sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2)));

    return delta; // Повернення обчисленої відстані
}

// Функція для створення снаряду
void CreateProjectile(Vector2D pos, Vector2D vel, int range, int speed, int dmg, const char* id, Manager* man, Entity& enemy, int type)
{
    // Створення з використанням менеджера
    auto& projectile(man->addEntity());
    // Передача позиції, розміру та швидкості
    projectile.addComponent<TransformComponent>(pos.x, pos.y, 16, 16, speed);
    // Передача шляху до текстури і вказівки, чи анімований спрайт
    projectile.addComponent<SpriteComponent>(id, false);
    // Передача дальності, швидкості, вектору швидкості, шкоди, цілі та типу снаряду
    projectile.addComponent<ProjectileComponent>(range, speed, vel, dmg, enemy, type);
    // Створення колайдера
    projectile.addComponent<ColliderComponent>();
    // Додавання до групи снарядів
    projectile.addGroup(Game::groupProjectiles);
}

// Функція для створення ворога
void CreateEnemy(Vector2D pos, Vector2D vel, int speed, int health, const char* id, Manager* man, int type)
{
    // Створення з використанням менеджера
    auto& enemy(man->addEntity());
    // Передача позиції, розміру та швидкості
    enemy.addComponent<TransformComponent>(pos.x, pos.y, 32, 32, speed);
    // Передача шляху до текстури і вказівки, чи анімований спрайт
    enemy.addComponent<SpriteComponent>(id, false);
    // Передача здоров'я, вектору швидкості та типу ворога
    enemy.addComponent<EnemyComponent>(health, vel, type);
    // Додавання колайдера
    enemy.addComponent<ColliderComponent>();
    // Додавання до групи ворогів
    enemy.addGroup(Game::groupEnemies);
}

// Функція для відтворення звуку
void playSound(const char* path) {
    Mix_Chunk* sound = Mix_LoadWAV(path); // Завантаження звуку з файлу
    if (sound == NULL) {
        std::cout << "Failed to load sound effect! SDL_mixer Error: " << Mix_GetError(); // Виведення помилки, якщо звук не вдалося завантажити
    }
    Mix_PlayChannel(-1, sound, 0); // Відтворення звуку
    // Mix_FreeChunk(sound); // Звільнення звуку (якщо потрібно)
}
