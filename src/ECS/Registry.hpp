//
//  Registry.hpp
//  GameTestSDL3
//
//  Created by Edgar Alamillo on 5/19/26.
//

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

class IComponentStorage {
public:
    virtual ~IComponentStorage() = default;
    virtual void erase(Entity entity) = 0;
};

template <typename T>
class ComponentStorage final : public IComponentStorage {
    static_assert(
        std::is_base_of_v<BaseComponent, T> &&
        !std::is_same_v<BaseComponent, T>,
        "ComponentStorage<T>: T must inherit from BaseComponent."
    );

public:
    std::unordered_map<Entity, T> data;

    void erase(Entity entity) override {
        data.erase(entity);
    }
};

class Registry
{
private:
    Entity nextEntity_ = 1;

    std::vector<Entity> aliveEntities_;

    std::unordered_map<std::type_index,
        std::unique_ptr<IComponentStorage>
    > componentStorages_;
public:
    Entity create() {
        Entity entity = nextEntity_++;
        aliveEntities_.push_back(entity);
        return entity;
    }

    void destroy(Entity entity)
    {
        aliveEntities_.erase(
            std::remove(aliveEntities_.begin(), aliveEntities_.end(), entity),
            aliveEntities_.end()
        );

        for (auto& [type, storage] : componentStorages_) {
            (void)type;
            storage->erase(entity);
        }
    }

    template <typename T>
    T& add(Entity entity, T component) {
        static_assert(
            std::is_base_of_v<BaseComponent, T> &&
            !std::is_same_v<BaseComponent, T>,
            "Registry::add<T>: T must inherit from BaseComponent."
        );

        auto& components = getStorage<T>().data;

        auto [it, inserted] = components.insert_or_assign(
            entity,
            std::move(component)
        );

        (void)inserted;

        return it->second;
    }

    template <typename T>
    T* get(Entity entity) {
        auto& components = getStorage<T>().data;

        auto it = components.find(entity);

        if (it == components.end()) {
            return nullptr;
        }

        return &it->second;
    }

    template <typename T>
    bool has(Entity entity) {
        return get<T>(entity) != nullptr;
    }

    template <typename T>
    void remove(Entity entity) {
        getStorage<T>().data.erase(entity);
    }

    template <typename T>
    std::unordered_map<Entity, T>& all() {
        return getStorage<T>().data;
    }

private:
    template <typename T>
    ComponentStorage<T>& getStorage() {
        static_assert(
            std::is_base_of_v<BaseComponent, T> &&
            !std::is_same_v<BaseComponent, T>,
            "Registry component type T must inherit from BaseComponent."
        );

        std::type_index type = std::type_index(typeid(T));

        auto it = componentStorages_.find(type);

        if (it == componentStorages_.end()) {
            auto storage = std::make_unique<ComponentStorage<T>>();
            auto* rawStorage = storage.get();

            componentStorages_.emplace(type, std::move(storage));

            return *rawStorage;
        }

        return static_cast<ComponentStorage<T>&>(*it->second);
    }


};


#endif /* Registry_hpp */
