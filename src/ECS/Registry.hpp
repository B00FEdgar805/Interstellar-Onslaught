#ifndef Registry_hpp
#define Registry_hpp

#include "Components/Component.hpp"
#include "Types.hpp"

#include <algorithm>
#include <memory>
#include <typeindex>
#include <type_traits>
#include <unordered_map>
#include <utility>
#include <vector>

class IComponentStorage // Base class for component storage
{
public:
    virtual ~IComponentStorage() = default;
    virtual void erase(Entity entity) = 0;
};

template <typename T>
class ComponentStorage final : public IComponentStorage // Makes sure new componets derive from bas component class and stores them in a map
{
    static_assert(std::is_base_of_v<BaseComponent, T> && !std::is_same_v<BaseComponent, T>,
        "ComponentStorage<T>: T must inherit from BaseComponent.");

public:
    std::unordered_map<Entity, T> data;

    void erase(Entity entity) override
    {
        data.erase(entity);
    }
};

class Registry
{
private:
    Entity NEXT_ENTITY = 1;

    std::vector<Entity> ALIVE_ENTITES;

    std::unordered_map<std::type_index,std::unique_ptr<IComponentStorage>> COMPONENT_STORAGES;
public:
    Entity create() // Adds new entities and puts them into the vector
    {
        Entity entity = NEXT_ENTITY++;
        ALIVE_ENTITES.push_back(entity);
        return entity;
    }

    void destroy(Entity entity) // Destorys entities and all of its components
    {
        ALIVE_ENTITES.erase(std::remove(ALIVE_ENTITES.begin(), ALIVE_ENTITES.end(), entity),ALIVE_ENTITES.end());

        for (auto& [type, storage] : COMPONENT_STORAGES)
        {
            (void)type;
            storage -> erase(entity);
        }
    }

    template <typename T>
    T& add(Entity entity, T component)  // Adds components to exisiting eneitiies and also checks if compoennts are valid
    {
        static_assert(std::is_base_of_v<BaseComponent, T> && !std::is_same_v<BaseComponent, T>,
            "Registry::add<T>: T must inherit from BaseComponent.");

        auto& components = getStorage<T>().data;

        auto [it, inserted] = components.insert_or_assign(entity, std::move(component));

        (void)inserted;

        return it->second;
    }

    template <typename T>
    T* get(Entity entity)   // Returns entities compoenet
    {
        auto& components = getStorage<T>().data;

        auto it = components.find(entity);

        if (it == components.end())
        {
            return nullptr;
        }

        return &it->second;
    }

    template <typename T>
    bool has(Entity entity)
    {
        return get<T>(entity) != nullptr;
    }

    template <typename T>
    void remove(Entity entity)
    {
        getStorage<T>().data.erase(entity);
    }

    template <typename T>
    std::unordered_map<Entity, T>& all() // Returns all entities compoentns
    {
        return getStorage<T>().data;
    }

private:
    template <typename T>
    ComponentStorage<T>& getStorage()
    {
        static_assert(std::is_base_of_v<BaseComponent, T> && !std::is_same_v<BaseComponent, T>,
            "Registry component type T must inherit from BaseComponent.");

        std::type_index type = std::type_index(typeid(T));

        auto it = COMPONENT_STORAGES.find(type);

        if (it == COMPONENT_STORAGES.end())
        {
            auto storage = std::make_unique<ComponentStorage<T>>();
            auto* rawStorage = storage.get();

            COMPONENT_STORAGES.emplace(type, std::move(storage));

            return *rawStorage;
        }

        return static_cast<ComponentStorage<T>&>(*it->second);
    }


};


#endif /* Registry_hpp */
