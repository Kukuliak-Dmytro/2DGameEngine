#include "Game.h"
Game* game = nullptr;

int main(int argc, char* argv[]) {
    // Встановлюємо кількість кадрів в секунду (FPS)
    const int FPS = 60;
    // Обчислюємо затримку між кадрами у мілісекундах
    const int frameDelay = 1000 / FPS;

    Uint32 frameStart; // Змінна для зберігання часу початку кадру
    int frameTime; // Змінна для зберігання часу, який знадобився для обробки кадру

    game = new Game(); // Створюємо новий об'єкт гри

    // Ініціалізуємо гру з вікном розміром 1920x1080, розташованим в центрі екрану
    game->init("Tower Defense", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1920, 1080, false);

    // Основний ігровий цикл
    while (game->running()) {
        frameStart = SDL_GetTicks(); // Отримуємо час початку кадру

        game->handleEvents(); // Обробка подій (наприклад, натискання клавіш, рух миші тощо)
        game->update(); // Оновлення стану гри (логіка гри)
        game->render(); // Відображення гри на екран

        frameTime = SDL_GetTicks() - frameStart; // Обчислюємо час, який знадобився для обробки кадру

        // Якщо час обробки кадру менший за встановлену затримку між кадрами, виконуємо затримку
        if (frameDelay > frameTime) {
            SDL_Delay(frameDelay - frameTime); // Затримка, щоб забезпечити постійну частоту кадрів
        }
    }

    game->clean(); // Очищення ресурсів гри
    delete game; // Видалення об'єкта гри
    return 0;
}
