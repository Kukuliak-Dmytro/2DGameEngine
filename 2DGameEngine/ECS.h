#pragma once  // Запобігає багаторазовому включенню цього заголовкового файлу

#include <algorithm>  // Включає бібліотеку для використання алгоритмів STL
#include <array>  // Включає бібліотеку для використання контейнеру array
#include <bitset>  // Включає бібліотеку для використання бітових множин (bitset)
#include <iostream>  // Включає бібліотеку для вводу/виводу
#include <memory>  // Включає бібліотеку для використання smart pointers (розумних вказівників)
#include <vector>  // Включає бібліотеку для використання контейнеру vector
#include "Game.h"

// Попереднє оголошення класів
class Component;  // Попереднє оголошення класу Component
class Entity;  // Попереднє оголошення класу Entity
class Manager;  // Попереднє оголошення класу Manager

// Використовувані псевдоніми типів
using ComponentID = std::size_t;  // Псевдонім для типу size_t, який використовується як ідентифікатор компоненту
using Group = std::size_t;  // Псевдонім для типу size_t, який використовується як ідентифікатор групи

// Функції для отримання унікальних ідентифікаторів компонентів
inline ComponentID getNewComponentTypeID() {
	static ComponentID LastID = 0u;  // Статична змінна, яка зберігає останній використаний ID
	return LastID++;  // Повертає новий унікальний ID
}

template <typename T> inline ComponentID getComponentTypeID() noexcept {
	static ComponentID typeID = getNewComponentTypeID();  // Статична змінна, яка зберігає ID для конкретного типу компоненту
	return typeID;  // Повертає ID для типу T
}

// Константи для обмежень
constexpr std::size_t maxComponents = 32;  // Максимальна кількість компонентів
constexpr std::size_t maxGroups = 32;  // Максимальна кількість груп

// Використані контейнери та типи
using ComponentBitSet = std::bitset<maxComponents>;  // Бітова множина для відстежування компонентів
using GroupBitSet = std::bitset<maxGroups>;  // Бітова множина для відстежування груп
using ComponentArray = std::vector<Component*>;  // Вектор вказівників на компоненти

// Клас компоненту
class Component {
public:
	Entity* entity;  // Вказівник на сутність, до якої належить компонент

	virtual void init() {}  // Віртуальна функція для ініціалізації компонента
	virtual void update() {}  // Віртуальна функція для оновлення компонента
	virtual void draw() {}  // Віртуальна функція для малювання компонента

	virtual ~Component() {}  // Віртуальний деструктор
};

// Клас сутності
class Entity {
private:
	Manager& manager;  // Посилання на менеджера
	bool active = true;  // Змінна, яка вказує на те, чи активна сутність
	std::vector<std::unique_ptr<Component>> components;  // Вектор унікальних вказівників на компоненти
	ComponentArray componentArray;  // Вектор вказівників на компоненти
	ComponentBitSet componentBitSet;  // Бітова множина для відстежування наявності компонентів
	GroupBitSet groupBitSet;  // Бітова множина для відстежування належності до груп

public:
	Entity(Manager& mManager) : manager(mManager) {
		componentArray.resize(maxComponents, nullptr);  // Ініціалізує вектор нульовими вказівниками
	}

	Manager& getManager() { return manager; }  // Повертає посилання на менеджера

	void update() {
		for (auto& c : components) c->update();  // Викликає метод update для всіх компонентів
	}

	void draw() {
		for (auto& c : components) c->draw();  // Викликає метод draw для всіх компонентів
	}

	bool isActive() const { return active; }  // Повертає значення активності сутності

	bool hasGroup(Group myGroup) {
		return groupBitSet[myGroup];  // Перевіряє, чи належить сутність до вказаної групи
	}

	void addGroup(Group myGroup);
	void delGroup(Group myGroup) {
		groupBitSet[myGroup] = false;  // Видаляє сутність з вказаної групи
	}

	void destroy() { active = false; }  // Деактивує сутність

	template <typename T> bool hasComponent() const {
		return componentBitSet[getComponentTypeID<T>()];  // Перевіряє наявність компоненту типу T
	}

	template <typename T, typename... TArgs>
	T& addComponent(TArgs&&... mArgs) {
		T* c = new T(std::forward<TArgs>(mArgs)...);  // Створює новий компонент типу T
		c->entity = this;  // Встановлює вказівник на сутність
		std::unique_ptr<Component> uPtr{ c };  // Створює унікальний вказівник на компонент
		components.emplace_back(std::move(uPtr));  // Додає компонент до вектора компонентів
		componentArray[getComponentTypeID<T>()] = c;  // Зберігає вказівник на компонент в масиві
		componentBitSet[getComponentTypeID<T>()] = true;  // Відмічає наявність компоненту в бітовій множині
		c->init();  // Ініціалізує компонент
		return *c;  // Повертає посилання на компонент
	}

	template <typename T>
	void removeComponent() {
		if (!hasComponent<T>()) {
			// Компонент не існує
			return;
		}
		auto& component = getComponent<T>();
		componentBitSet[getComponentTypeID<T>()] = false;  // Відмічає відсутність компоненту в бітовій множині

		// Видалення компонента з вектора компонентів
		auto it = std::find_if(components.begin(), components.end(), [&](const std::unique_ptr<Component>& ptr) {
			return ptr.get() == &component;
			});

		if (it != components.end()) {
			components.erase(it);  // Видаляє компонент з вектора
		}
		componentArray[getComponentTypeID<T>()] = nullptr;  // Зануляє вказівник в масиві
	}

	template <typename T> T& getComponent() const {
		auto ptr = componentArray[getComponentTypeID<T>()];
		return *static_cast<T*>(ptr);  // Повертає посилання на компонент типу T
	}
};

// Клас менеджера
class Manager {
private:
	std::vector<std::unique_ptr<Entity>> entities;  // Вектор унікальних вказівників на сутності
	std::array<std::vector<Entity*>, maxGroups> groupedEntities;  // Масив векторів вказівників на сутності по групах

public:
	void update() {
		for (auto& e : entities) e->update();  // Викликає метод update для всіх сутностей
	}

	void draw() {
		for (auto& e : entities) e->draw();  // Викликає метод draw для всіх сутностей
	}

	void refresh() {
		// Проходимо через всі групи
		for (auto i(0u); i < maxGroups; i++) {
			auto& v(groupedEntities[i]);  // Отримуємо вектор сутностей, які належать до групи i
			v.erase(
				// Видаляємо всі сутності, які неактивні або більше не належать до групи i
				std::remove_if(std::begin(v), std::end(v),
					[i](Entity* mEntity) {  // Лямбда-функція для перевірки умови
						return !mEntity->isActive() || !mEntity->hasGroup(i);  // Перевіряємо, чи сутність неактивна або не належить до групи i
					}
				),
				std::end(v)  // Останній елемент вектора
			);
		}
		// Видаляємо всі неактивні сутності з вектора entities
		entities.erase(
			std::remove_if(std::begin(entities), std::end(entities),
				[](const std::unique_ptr<Entity>& mEntity) {  // Лямбда-функція для перевірки умови
					return !mEntity->isActive();  // Перевіряємо, чи сутність неактивна
				}
			),
			std::end(entities)  // Останній елемент вектора
		);
	}


	void AddToGroup(Entity* myEntity, Group myGroup) {
		groupedEntities[myGroup].emplace_back(myEntity);  // Додає сутність до вказаної групи
	}

	std::vector<Entity*>& getGroup(Group myGroup) {
		return groupedEntities[myGroup];  // Повертає вектор сутностей, які належать до вказаної групи
	}

	Entity& addEntity() {
		Entity* e = new Entity(*this);  // Створює нову сутність
		std::unique_ptr<Entity> uPtr{ e };  // Створює унікальний вказівник на сутність
		entities.emplace_back(std::move(uPtr));  // Додає сутність до вектора сутностей
		return *e;  // Повертає посилання на нову сутність
	}
};
