#pragma once  // ������� �������������� ��������� ����� ������������� �����

#include <algorithm>  // ������ �������� ��� ������������ ��������� STL
#include <array>  // ������ �������� ��� ������������ ���������� array
#include <bitset>  // ������ �������� ��� ������������ ������ ������ (bitset)
#include <iostream>  // ������ �������� ��� �����/������
#include <memory>  // ������ �������� ��� ������������ smart pointers (�������� ���������)
#include <vector>  // ������ �������� ��� ������������ ���������� vector
#include "Game.h"

// �������� ���������� �����
class Component;  // �������� ���������� ����� Component
class Entity;  // �������� ���������� ����� Entity
class Manager;  // �������� ���������� ����� Manager

// �������������� ��������� ����
using ComponentID = std::size_t;  // �������� ��� ���� size_t, ���� ��������������� �� ������������� ����������
using Group = std::size_t;  // �������� ��� ���� size_t, ���� ��������������� �� ������������� �����

// ������� ��� ��������� ��������� �������������� ����������
inline ComponentID getNewComponentTypeID() {
	static ComponentID LastID = 0u;  // �������� �����, ��� ������ ������� ������������ ID
	return LastID++;  // ������� ����� ��������� ID
}

template <typename T> inline ComponentID getComponentTypeID() noexcept {
	static ComponentID typeID = getNewComponentTypeID();  // �������� �����, ��� ������ ID ��� ����������� ���� ����������
	return typeID;  // ������� ID ��� ���� T
}

// ��������� ��� ��������
constexpr std::size_t maxComponents = 32;  // ����������� ������� ����������
constexpr std::size_t maxGroups = 32;  // ����������� ������� ����

// ���������� ���������� �� ����
using ComponentBitSet = std::bitset<maxComponents>;  // ������ ������� ��� ������������ ����������
using GroupBitSet = std::bitset<maxGroups>;  // ������ ������� ��� ������������ ����
using ComponentArray = std::vector<Component*>;  // ������ ��������� �� ����������

// ���� ����������
class Component {
public:
	Entity* entity;  // �������� �� �������, �� ��� �������� ���������

	virtual void init() {}  // ³�������� ������� ��� ����������� ����������
	virtual void update() {}  // ³�������� ������� ��� ��������� ����������
	virtual void draw() {}  // ³�������� ������� ��� ��������� ����������

	virtual ~Component() {}  // ³��������� ����������
};

// ���� �������
class Entity {
private:
	Manager& manager;  // ��������� �� ���������
	bool active = true;  // �����, ��� ����� �� ��, �� ������� �������
	std::vector<std::unique_ptr<Component>> components;  // ������ ��������� ��������� �� ����������
	ComponentArray componentArray;  // ������ ��������� �� ����������
	ComponentBitSet componentBitSet;  // ������ ������� ��� ������������ �������� ����������
	GroupBitSet groupBitSet;  // ������ ������� ��� ������������ ��������� �� ����

public:
	Entity(Manager& mManager) : manager(mManager) {
		componentArray.resize(maxComponents, nullptr);  // �������� ������ ��������� �����������
	}

	Manager& getManager() { return manager; }  // ������� ��������� �� ���������

	void update() {
		for (auto& c : components) c->update();  // ������� ����� update ��� ��� ����������
	}

	void draw() {
		for (auto& c : components) c->draw();  // ������� ����� draw ��� ��� ����������
	}

	bool isActive() const { return active; }  // ������� �������� ��������� �������

	bool hasGroup(Group myGroup) {
		return groupBitSet[myGroup];  // ��������, �� �������� ������� �� ������� �����
	}

	void addGroup(Group myGroup);
	void delGroup(Group myGroup) {
		groupBitSet[myGroup] = false;  // ������� ������� � ������� �����
	}

	void destroy() { active = false; }  // �������� �������

	template <typename T> bool hasComponent() const {
		return componentBitSet[getComponentTypeID<T>()];  // �������� �������� ���������� ���� T
	}

	template <typename T, typename... TArgs>
	T& addComponent(TArgs&&... mArgs) {
		T* c = new T(std::forward<TArgs>(mArgs)...);  // ������� ����� ��������� ���� T
		c->entity = this;  // ���������� �������� �� �������
		std::unique_ptr<Component> uPtr{ c };  // ������� ��������� �������� �� ���������
		components.emplace_back(std::move(uPtr));  // ���� ��������� �� ������� ����������
		componentArray[getComponentTypeID<T>()] = c;  // ������ �������� �� ��������� � �����
		componentBitSet[getComponentTypeID<T>()] = true;  // ³���� �������� ���������� � ����� ������
		c->init();  // �������� ���������
		return *c;  // ������� ��������� �� ���������
	}

	template <typename T>
	void removeComponent() {
		if (!hasComponent<T>()) {
			// ��������� �� ����
			return;
		}
		auto& component = getComponent<T>();
		componentBitSet[getComponentTypeID<T>()] = false;  // ³���� ��������� ���������� � ����� ������

		// ��������� ���������� � ������� ����������
		auto it = std::find_if(components.begin(), components.end(), [&](const std::unique_ptr<Component>& ptr) {
			return ptr.get() == &component;
			});

		if (it != components.end()) {
			components.erase(it);  // ������� ��������� � �������
		}
		componentArray[getComponentTypeID<T>()] = nullptr;  // ������� �������� � �����
	}

	template <typename T> T& getComponent() const {
		auto ptr = componentArray[getComponentTypeID<T>()];
		return *static_cast<T*>(ptr);  // ������� ��������� �� ��������� ���� T
	}
};

// ���� ���������
class Manager {
private:
	std::vector<std::unique_ptr<Entity>> entities;  // ������ ��������� ��������� �� �������
	std::array<std::vector<Entity*>, maxGroups> groupedEntities;  // ����� ������� ��������� �� ������� �� ������

public:
	void update() {
		for (auto& e : entities) e->update();  // ������� ����� update ��� ��� ���������
	}

	void draw() {
		for (auto& e : entities) e->draw();  // ������� ����� draw ��� ��� ���������
	}

	void refresh() {
		// ��������� ����� �� �����
		for (auto i(0u); i < maxGroups; i++) {
			auto& v(groupedEntities[i]);  // �������� ������ ���������, �� �������� �� ����� i
			v.erase(
				// ��������� �� �������, �� �������� ��� ����� �� �������� �� ����� i
				std::remove_if(std::begin(v), std::end(v),
					[i](Entity* mEntity) {  // ������-������� ��� �������� �����
						return !mEntity->isActive() || !mEntity->hasGroup(i);  // ����������, �� ������� ��������� ��� �� �������� �� ����� i
					}
				),
				std::end(v)  // ������� ������� �������
			);
		}
		// ��������� �� �������� ������� � ������� entities
		entities.erase(
			std::remove_if(std::begin(entities), std::end(entities),
				[](const std::unique_ptr<Entity>& mEntity) {  // ������-������� ��� �������� �����
					return !mEntity->isActive();  // ����������, �� ������� ���������
				}
			),
			std::end(entities)  // ������� ������� �������
		);
	}


	void AddToGroup(Entity* myEntity, Group myGroup) {
		groupedEntities[myGroup].emplace_back(myEntity);  // ���� ������� �� ������� �����
	}

	std::vector<Entity*>& getGroup(Group myGroup) {
		return groupedEntities[myGroup];  // ������� ������ ���������, �� �������� �� ������� �����
	}

	Entity& addEntity() {
		Entity* e = new Entity(*this);  // ������� ���� �������
		std::unique_ptr<Entity> uPtr{ e };  // ������� ��������� �������� �� �������
		entities.emplace_back(std::move(uPtr));  // ���� ������� �� ������� ���������
		return *e;  // ������� ��������� �� ���� �������
	}
};
