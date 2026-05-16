//
//  ECS.hpp
//  GameTestSDL3
//
//  Created by Edgar Alamillo on 5/15/26.
//

#ifndef ECS_hpp
#define ECS_hpp

#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <bitset>
#include <array>

class Component;
class Entity;

using ComponentID = std::size_t;

inline ComponentID getComponentTypeID()
{
    static ComponentID LastID = 0;
    return LastID++;
}

template <typename T> inline ComponentID getComponentTypeID() noexcept
{
    static ComponentID typeID = getComponentTypeID();
    return typeID;
}

constexpr std::size_t maxComponents = 32;

using ComponentBitSet = std::bitset<maxComponents>;
using ComponentArray = std::array<Component*, maxComponents>;

class Component
{
private:
    
public:
    Entity* entity;
    virtual void init() {}
    virtual void update() {}
    virtual void draw() {}
    
    virtual ~Component() {}
    
};

class Entity
{
private:
    bool ACTIVE = false;
    std::vector<std::unique_ptr<Component>> COMPONENTS;
    
    ComponentArray COMPONENT_ARRAY;
    ComponentBitSet COMPONENT_BIT_SET;
public:
    void update()
    {
        for(auto &c: COMPONENTS)
        {
            c -> update();
        }
        
        for(auto &c: COMPONENTS)
        {
            c -> draw();
        }
    }
    
    void draw() {}
    bool isActive() const
    {
        return ACTIVE;
    }
    void destory()
    {
        ACTIVE = false;
    }
    
    template <typename T> bool hasComponent() const
    {
        return COMPONENT_BIT_SET[getComponentTypeID<T>];
    }
    
    template <typename T, typename... TArgs>
    T& addComponent(TArgs&&... mArgs)
    {
        T* c(new T(std::forward<TArgs>(mArgs)...));
        c -> entity = this;
        std::unique_ptr<Component> uPtr{ c };
        COMPONENTS.emplace_back(std::move(uPtr));
        COMPONENT_ARRAY[getComponentTypeID<T>()] = c;
        COMPONENT_BIT_SET[getComponentTypeID<T>()] = true;
        
        c -> init();
        return *c;
    }
    
    template <typename T> T& getComponent() const
    {
        auto ptr(COMPONENT_ARRAY[getComponentTypeID<T>()]);
        return *static_cast<T*>(ptr);
    }
    
};
class Manager
{
private:
    std::vector<std::unique_ptr<Entity>> ENTITIES;
public:
    void update()
    {
        for(auto &e: ENTITIES)
        {
            e -> update();
        }
    }
    
    void draw()
    {
        for(auto &e: ENTITIES)
        {
            e -> draw();
        }
    }
    
    void refresh()
    {
        ENTITIES.erase(std::remove_if(std::begin(ENTITIES), std::end(ENTITIES), [](const std::unique_ptr<Entity> &mEntity)
        {
            return !mEntity -> isActive();
        }),
                       std::end(ENTITIES));
    }
    
    Entity& addEntity()
    {
        Entity* e = new Entity();
        std::unique_ptr<Entity> uPtr(e);
        ENTITIES.emplace_back(std::move(uPtr));
        return *e;
    }
};


#endif /* ECS_hpp */
