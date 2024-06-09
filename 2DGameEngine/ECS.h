#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <bitset>
#include <array>

// Попереднє оголошення класів
class Component;
class Entity;
class Manager;

// Використовувані псевдоніми типів
using ComponentID = std::size_t;
using Group = std::size_t;

// Функції для отримання унікальних ідентифікаторів компонентів
inline ComponentID getNewComponentTypeID() {
    static ComponentID LastID = 0u;
    return LastID++;
}

template <typename T> inline ComponentID getComponentTypeID() noexcept {
    static ComponentID typeID = getNewComponentTypeID();
    return typeID;
}

// Константи для обмежень
constexpr std::size_t maxComponents = 32;
constexpr std::size_t maxGroups = 32;

// Використані контейнери та типи
using ComponentBitSet = std::bitset<maxComponents>;
using GroupBitSet = std::bitset<maxGroups>;
using ComponentArray = std::array<Component*, maxComponents>;

// Клас компоненту
class Component {
public:
    Entity* entity;

    virtual void init() {}
    virtual void update() {}
    virtual void draw() {}

    virtual ~Component() {}
};

// Клас сутності
class Entity {
private:
    Manager& manager;
    bool active = true;
    std::vector<std::unique_ptr<Component>> components;
    ComponentArray componentArray;
    ComponentBitSet componentBitSet;
    GroupBitSet groupBitSet;

public:
    Entity(Manager& mManager) : manager(mManager) {};
    Manager& getManager() { return manager; }

    void update() {
        for (auto& c : components) c->update();
    }

    void draw() {
        for (auto& c : components) c->draw();
    }

    bool isActive() const { return active; }

    bool hasGroup(Group myGroup) {
        return groupBitSet[myGroup];
    }

    void addGroup(Group myGroup);
    void delGroup(Group myGroup) {
        groupBitSet[myGroup] = false;
    }

    void destroy() { active = false; }

    template <typename T> bool hasComponent() const {
        return componentBitSet[getComponentTypeID<T>()];
    }

    template <typename T, typename... TArgs>
    T& addComponent(TArgs&&... mArgs) {
        T* c = new T(std::forward<TArgs>(mArgs)...);
        c->entity = this;
        std::unique_ptr<Component> uPtr{ c };
        components.emplace_back(std::move(uPtr));
        componentArray[getComponentTypeID<T>()] = c;
        componentBitSet[getComponentTypeID<T>()] = true;
        c->init();
        return *c;
    }

    template <typename T>
    void removeComponent() {
        if (!hasComponent<T>()) {
            // Компонент не існує
            return;
        }
        auto& component = getComponent<T>();
        componentBitSet[getComponentTypeID<T>()] = false;

        // Видалення компонента з вектора компонентів
        auto it = std::find_if(components.begin(), components.end(), [&](const std::unique_ptr<Component>& ptr) {
            return ptr.get() == &component;
            });

        if (it != components.end()) {
            components.erase(it);
        }
    }

    template <typename T> T& getComponent() const {
        auto ptr = componentArray[getComponentTypeID<T>()];
        return *static_cast<T*>(ptr);
    }
};

// Клас менеджера
class Manager {
private:
    std::vector<std::unique_ptr<Entity>> entities;
    std::array<std::vector<Entity*>, maxGroups> groupedEntities;

public:
    void update() {
        for (auto& e : entities) e->update();
    }

    void draw() {
        for (auto& e : entities) e->draw();
    }

   

    void refresh() {
        for (auto i(0u); i < maxGroups; i++) {
            auto& v(groupedEntities[i]);
            v.erase(
                std::remove_if(std::begin(v), std::end(v),
                    [i](Entity* mEntity) {
                        return !mEntity->isActive() || !mEntity->hasGroup(i);
                    }
                ),
                std::end(v)
                        );
        }
        entities.erase(std::remove_if(std::begin(entities), std::end(entities), [](const std::unique_ptr<Entity>& mEntity) {
            return !mEntity->isActive();
            }),
            std::end(entities));
    }

    void AddToGroup(Entity* myEntity, Group myGroup) {
        groupedEntities[myGroup].emplace_back(myEntity);
    }

    std::vector<Entity*>& getGroup(Group myGroup) {
        return groupedEntities[myGroup];
    }

    Entity& addEntity() {
        Entity* e = new Entity(*this);
        std::unique_ptr<Entity> uPtr{ e };
        entities.emplace_back(std::move(uPtr));
        return *e;
    }
};
