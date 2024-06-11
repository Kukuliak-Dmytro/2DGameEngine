#pragma once  // Забезпечує одноразове включення заголовочного файлу

#include <iostream>  // Підключення бібліотеки для вводу/виводу

class Vector2D {
public:
    float x;  // Координата x вектора
    float y;  // Координата y вектора

    // Конструктор за замовчуванням
    Vector2D();
    // Конструктор з параметрами
    Vector2D(float x, float y);

    // Метод для нормалізації вектора( встановити довжину вектора =1)
    void normalize();
    // Перевантажений метод для нормалізації( встановити довжину вектора =а)
    void normalize(int a); 

    // Метод для додавання іншого вектора
    Vector2D& Add(const Vector2D& vector);
    // Метод для віднімання іншого вектора
    Vector2D& Subtract(const Vector2D& vector);
    // Метод для ділення на інший вектор
    Vector2D& Divide(const Vector2D& vector);
    // Метод для множення на інший вектор
    Vector2D& Multiply(const Vector2D& vector);

    // Дружні функції для перевантаження операторів
    friend Vector2D& operator+(Vector2D& v1, Vector2D& v2);
    friend Vector2D& operator-(Vector2D& v1, Vector2D& v2);
    friend Vector2D& operator/(Vector2D& v1, Vector2D& v2);
    friend Vector2D& operator*(Vector2D& v1, Vector2D& v2);

    // Перевантаження операторів для додавання з присвоєнням
    Vector2D& operator+=(Vector2D& vec);
    // Перевантаження оператора присвоєння
    Vector2D& operator=(Vector2D& vec);
    // Перевантаження операторів для віднімання з присвоєнням
    Vector2D& operator-=(Vector2D& vec);
    // Перевантаження операторів для ділення з присвоєнням
    Vector2D& operator/=(Vector2D& vec);
    // Перевантаження операторів для множення з присвоєнням
    Vector2D& operator*=(Vector2D& vec);

    // Перевантаження оператора для множення на скаляр (ціле число)
    Vector2D& operator*(const int& i);

    // Метод для встановлення координат вектора в нуль
    Vector2D& Zero();

    // Дружня функція для перевантаження оператора виводу
    friend std::ostream& operator<<(std::ostream& stream, const Vector2D& vec);
};
