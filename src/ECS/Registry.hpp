#ifndef Registry_hpp
#define Registry_hpp

#include "Components/Component.hpp"
#include "Types.hpp"

#include <algorithm>
#include <bitset> 
#include <cstddef>
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
class ComponentStorage final : public IComponentStorage // Sparse set storage for components
{
    static_assert(std::is_base_of_v<BaseComponent, T> && !std::is_same_v<BaseComponent, T>,
        "ComponentStorage<T>: T must inherit from BaseComponent.");

public:
    std::vector<T> denseComponents;
    std::vector<Entity> denseEntities;
    std::vector<size_t> sparse; // Indexed by entity; SIZE_MAX if entity not present

    void erase(Entity entity) override
    {
        if (entity >= sparse.size() || sparse[entity] == SIZE_MAX) return;

        size_t idx = sparse[entity];
        size_t last = denseComponents.size() - 1;

        if (idx != last)
        {
            denseComponents[idx] = std::move(denseComponents[last]);
            denseEntities[idx] = denseEntities[last];
            sparse[denseEntities[idx]] = idx;
        }

        denseComponents.pop_back();
        denseEntities.pop_back();
        sparse[entity] = SIZE_MAX;
    }
};

class Registry
{
private:
    // Maximum number of unique component types supported
    static constexpr size_t MAX_COMPONENTS = 64;

    Entity NEXT_ENTITY = 1;

    std::vector<Entity> ALIVE_ENTITES;

    std::unordered_map<std::type_index,std::unique_ptr<IComponentStorage>> COMPONENT_STORAGES;

    using Signature = std::bitset<MAX_COMPONENTS>;

    std::vector<Signature> ENTITY_SIGNATURES; // Signature by Entity ID

    // Static method to get unique index for each component type T
    template<typename T>
    static size_t componentTypeIndex() {
        static size_t index = nextComponentTypeIndex();
        return index;
    }

    // Static method to generate next unique component type index
    static size_t nextComponentTypeIndex() {
        static size_t value = 0;
        return value++;
    }

public:
    Entity create() // Adds new entities and puts them into the vector
    {
        Entity entity = NEXT_ENTITY++;
        ALIVE_ENTITES.push_back(entity);

        // Ensure ENTITY_SIGNATURES vector is large enough, and reset the signature bitset for new entity
        if (entity >= ENTITY_SIGNATURES.size()) ENTITY_SIGNATURES.resize(entity + 1);
        ENTITY_SIGNATURES[entity].reset();

        return entity;
    }

    void destroy(Entity entity) // Destroys entities and all of its components
    {
        auto it = std::find(ALIVE_ENTITES.begin(), ALIVE_ENTITES.end(), entity);
        if (it != ALIVE_ENTITES.end())
        {
            *it = ALIVE_ENTITES.back();
            ALIVE_ENTITES.pop_back();
        }

        for (auto& [type, storage] : COMPONENT_STORAGES)
        {
            (void)type;
            storage->erase(entity);
        }

        // Reset signature for destroyed entity if within bounds
        if (entity < ENTITY_SIGNATURES.size()) ENTITY_SIGNATURES[entity].reset();
    }

    template <typename T>
    T& add(Entity entity, T component)  // Adds components to existing entities and checks if components are valid
    {
        static_assert(std::is_base_of_v<BaseComponent, T> && !std::is_same_v<BaseComponent, T>,
            "Registry::add<T>: T must inherit from BaseComponent.");

        auto& storage = getStorage<T>();

        if (entity >= storage.sparse.size())
            storage.sparse.resize(entity + 1, SIZE_MAX);

        if (storage.sparse[entity] != SIZE_MAX)
        {
            storage.denseComponents[storage.sparse[entity]] = std::move(component);

            // Set component bit in entity signature after updating component
            ENTITY_SIGNATURES[entity].set(componentTypeIndex<T>());

            return storage.denseComponents[storage.sparse[entity]];
        }
        else
        {
            storage.sparse[entity] = storage.denseComponents.size();
            storage.denseEntities.push_back(entity);
            storage.denseComponents.push_back(std::move(component));

            // Set component bit in entity signature after adding component
            ENTITY_SIGNATURES[entity].set(componentTypeIndex<T>());

            return storage.denseComponents.back();
        }
    }

    template <typename T>
    T* get(Entity entity)   // Returns entity's component pointer or nullptr
    {
        auto& storage = getStorage<T>();

        if (entity >= storage.sparse.size() || storage.sparse[entity] == SIZE_MAX)
            return nullptr;

        return &storage.denseComponents[storage.sparse[entity]];
    }

    template <typename T>
    bool has(Entity entity)
    {
        return get<T>(entity) != nullptr;
    }

    template <typename T>
    void remove(Entity entity)
    {
        getStorage<T>().erase(entity);

        // Clear component bit in entity signature after removal
        if (entity < ENTITY_SIGNATURES.size())
            ENTITY_SIGNATURES[entity].reset(componentTypeIndex<T>());
    }

    template <typename T>
    struct DenseView {
        std::vector<Entity>& entities;
        std::vector<T>& components;
    };

    // Returns all components and their associated entities in a dense view for iteration
    template <typename T>
    DenseView<T> all()
    {
        auto& storage = getStorage<T>();
        return DenseView<T>{storage.denseEntities, storage.denseComponents};
    }

    // Public accessor for getting the signature bitset of an entity
    const Signature& signature(Entity entity) const {
        return ENTITY_SIGNATURES[entity];
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
