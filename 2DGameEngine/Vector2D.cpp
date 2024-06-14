
#include "Vector2D.h"

// Конструктор за замовчуванням, ініціалізує вектор (0,0)
Vector2D::Vector2D()
{
	x = 0.0f;
	y = 0.0f;
}

// Конструктор з параметрами, ініціалізує вектор заданими значеннями
Vector2D::Vector2D(float x, float y)
{
	this->x = x;
	this->y = y;
}

// Метод для додавання вектора
Vector2D& Vector2D::Add(const Vector2D& vec)
{
	this->x += vec.x;
	this->y += vec.y;

	return *this;
}

// Метод для віднімання вектора
Vector2D& Vector2D::Subtract(const Vector2D& vec)
{
	this->x -= vec.x;
	this->y -= vec.y;

	return *this;
}

// Метод для ділення вектора
Vector2D& Vector2D::Divide(const Vector2D& vec)
{
	this->x /= vec.x;
	this->y /= vec.y;

	return *this;
}

// Метод для множення вектора
Vector2D& Vector2D::Multiply(const Vector2D& vec)
{
	this->x *= vec.x;
	this->y *= vec.y;

	return *this;
}

// Оператор додавання для двох векторів
Vector2D& operator+(Vector2D& v1, Vector2D& v2)
{
	return v1.Add(v2);
}

// Оператор віднімання для двох векторів
Vector2D& operator-(Vector2D& v1, Vector2D& v2)
{
	return v1.Subtract(v2);
}

// Оператор ділення для двох векторів
Vector2D& operator/(Vector2D& v1, Vector2D& v2)
{
	return v1.Divide(v2);
}

// Оператор множення для двох векторів
Vector2D& operator*(Vector2D& v1, Vector2D& v2)
{
	return v1.Multiply(v2);
}

// Оператор додавання для поточного вектора
Vector2D& Vector2D::operator+=(Vector2D& vec)
{
	return this->Add(vec);
}

// Оператор присвоєння для поточного вектора
Vector2D& Vector2D::operator=(Vector2D& vec)
{
	if (this != &vec) {
		this->x = vec.x;
		this->y = vec.y;
	}
	return *this;
}

// Оператор віднімання для поточного вектора
Vector2D& Vector2D::operator-=(Vector2D& vec)
{
	return this->Subtract(vec);
}

// Оператор ділення для поточного вектора
Vector2D& Vector2D::operator/=(Vector2D& vec)
{
	return this->Divide(vec);
}

// Оператор множення для поточного вектора
Vector2D& Vector2D::operator*=(Vector2D& vec)
{
	return this->Multiply(vec);
}

// Оператор множення для поточного вектора з цілочисельним множником
Vector2D& Vector2D::operator*(const int& i)
{
	this->x *= i;
	this->y *= i;
	return *this;
}

// Метод для обнулення вектора
Vector2D& Vector2D::Zero()
{
	this->x = 0;
	this->y = 0;
	return *this;
}

// Оператор виводу вектора
std::ostream& operator<<(std::ostream& stream, const Vector2D& vec) {
	stream << "(" << vec.x << "," << vec.y << ")";
	return stream;
}

// Метод для нормалізації вектора - переведення його довжини в 1, але зберігаючи напрям
void Vector2D::normalize() {
	float magnitude = sqrt(x * x + y * y);

	if (magnitude != 0) {
		x /= magnitude;
		y /= magnitude;
	}
}

// Метод для нормалізації вектора за параметром - переведення його довжини в параметр а, зберігаючи напрям
void Vector2D::normalize(int a) {
	float magnitude = sqrt(x * x + y * y);

	if (magnitude != 0) {
		x /= magnitude;
		y /= magnitude;
	}
	x = x * a;
	y = y * a;
}
