#include "CollisionSystem.hpp"
#include "../Globals.hpp"
#include "Components/BoxCollider.hpp"
#include "Components/Transform.hpp"
#include "Components/Velocity.hpp"

#include <SDL3/SDL.h>

#include <algorithm>
#include <iterator>

#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <cstdint>
#include <cmath>

namespace
{
    SDL_FRect makeWorldRect(const Transform& transform, const BoxCollider& collider)
    {
        SDL_FRect rect;

        rect.x = transform.position.x + collider.offset.x;
        rect.y = transform.position.y + collider.offset.y;
        rect.w = collider.size.x;
        rect.h = collider.size.y;

        return rect;
    }

    bool intersects(const SDL_FRect& a, const SDL_FRect& b)
    {
        
        return (
            a.x < b.x + b.w &&
            a.x + a.w > b.x &&
            a.y < b.y + b.h &&
            a.y + a.h > b.y
        );
        
        //return SDL_HasRectIntersectionFloat(&a, &b);
    }

    bool shouldCheckCollision(const BoxCollider& a, const BoxCollider& b)
    {
        bool aCanHitB = (a.collidesWith & b.layer) != 0;
        bool bCanHitA = (b.collidesWith & a.layer) != 0;

        return aCanHitB && bCanHitA;
    }

    CollisionEvent createCollisionEvent(Entity entityA, Entity entityB, const SDL_FRect& a, const SDL_FRect& b,const BoxCollider& colliderA, const BoxCollider& colliderB)
    {
        CollisionEvent event;

        event.a = entityA;
        event.b = entityB;
        event.isTrigger = colliderA.isTrigger || colliderB.isTrigger;

        float aLeft = a.x;
        float aRight = a.x + a.w;
        float aTop = a.y;
        float aBottom = a.y + a.h;

        float bLeft = b.x;
        float bRight = b.x + b.w;
        float bTop = b.y;
        float bBottom = b.y + b.h;

        float moveLeft = aRight - bLeft;
        float moveRight = bRight - aLeft;
        float moveUp = aBottom - bTop;
        float moveDown = bBottom - aTop;

        float minX = std::min(moveLeft, moveRight);
        float minY = std::min(moveUp, moveDown);

        if (minX < minY)
        {
            event.depth = minX;

            if (moveLeft < moveRight)
            {
                event.normal = Vector2D(-1.0f, 0.0f);
            }
            else
            {
                event.normal = Vector2D(1.0f, 0.0f);
            }
        }
        
        else
        {
            event.depth = minY;

            if (moveUp < moveDown)
            {
                event.normal = Vector2D(0.0f, -1.0f);
            }
            else
            {
                event.normal = Vector2D(0.0f, 1.0f);
            }
        }

        return event;
    }

    void moveEntity(Entity entity, const Vector2D& amount)
    {
        Transform* transform = GLOBALS::REGISTRY.get<Transform>(entity);

        if (transform == nullptr)
        {
            return;
        }

        transform -> position.x += amount.x;
        transform -> position.y += amount.y;
    }

    void stopVelocityOnCollisionAxis(Entity entity, const Vector2D& normal)
    {
        Velocity* velocity = GLOBALS::REGISTRY.get<Velocity>(entity);

        if (velocity == nullptr)
        {
            return;
        }

        if (normal.x != 0.0f)
        {
            velocity -> value.x = 0.0f;
        }

        if (normal.y != 0.0f)
        {
            velocity -> value.y = 0.0f;
        }
    }

    void resolveCollision(const CollisionEvent& event, const BoxCollider& colliderA, const BoxCollider& colliderB)
    {
        /*
        if (colliderA.tag == "enemy" && colliderB.tag == "enemy")
        {
            return;
        }
        */
        
        
        
        if (event.isTrigger)
        {
            return;
        }

        if (colliderA.isStatic && colliderB.isStatic)
        {
            return;
        }

        Vector2D correctionA(event.normal.x * event.depth, event.normal.y * event.depth);

        Vector2D correctionB(-event.normal.x * event.depth, -event.normal.y * event.depth);

        if (!colliderA.isStatic && colliderB.isStatic)
        {
            moveEntity(event.a, correctionA);
            stopVelocityOnCollisionAxis(event.a, event.normal);
            return;
        }

        if (colliderA.isStatic && !colliderB.isStatic)
        {
            moveEntity(event.b, correctionB);
            stopVelocityOnCollisionAxis(event.b, event.normal);
            return;
        }

        Vector2D halfCorrectionA(correctionA.x * 0.5f,correctionA.y * 0.5f);

        Vector2D halfCorrectionB(correctionB.x * 0.5f, correctionB.y * 0.5f);

        moveEntity(event.a, halfCorrectionA);
        moveEntity(event.b, halfCorrectionB);

        stopVelocityOnCollisionAxis(event.a, event.normal);
        stopVelocityOnCollisionAxis(event.b, event.normal);
    }

    // Grid cell size for partitioning
    constexpr float GRID_CELL_SIZE = 128.0f;

    struct GridCell
    {
        int x, y;
        bool operator==(const GridCell& o) const { return x == o.x && y == o.y; }
    };

}

namespace std
{
    template <>
    struct hash<GridCell>
    {
        size_t operator()(const GridCell& cell) const
        {
            return hash<int>()(cell.x) ^ (hash<int>()(cell.y) << 1);
        }
    };
}

namespace
{
    template <typename F>
    inline void forEachCell(const SDL_FRect& rect, F&& f)
    {
        const int x0 = (int)std::floor(rect.x / GRID_CELL_SIZE);
        const int y0 = (int)std::floor(rect.y / GRID_CELL_SIZE);
        const int x1 = (int)std::floor((rect.x + rect.w) / GRID_CELL_SIZE);
        const int y1 = (int)std::floor((rect.y + rect.h) / GRID_CELL_SIZE);
        for (int x = x0; x <= x1; ++x)
        {
            for (int y = y0; y <= y1; ++y)
            {
                f(GridCell{x, y});
            }
        }
    }

    inline uint64_t packPairKey(size_t a, size_t b)
    {
        if (a > b) std::swap(a, b);
        return (uint64_t(uint32_t(a)) << 32) | uint64_t(uint32_t(b));
    }
}

void collisionSystem(std::vector<CollisionEvent>& outCollisions, bool resolveSolidCollisions)
{
    outCollisions.clear();

    auto colliders = GLOBALS::REGISTRY.all<BoxCollider>();
    const size_t n = colliders.entities.size();
    outCollisions.reserve(n);
    if (n <= 1) return;

    // --- Spatial Hashing ---
    std::unordered_map<GridCell, std::vector<size_t>> grid; // GridCell indices in colliders array
    grid.reserve(n * 2);
    std::vector<SDL_FRect> rects(n);
    for (size_t i = 0; i < n; ++i)
    {
        Entity entity = colliders.entities[i];
        BoxCollider& collider = colliders.components[i];
        Transform* transform = GLOBALS::REGISTRY.get<Transform>(entity);
        if (!transform)
        {
            continue;
        }
        rects[i] = makeWorldRect(*transform, collider);
        forEachCell(rects[i], [&](const GridCell& cell)
        {
            grid[cell].push_back(i);
        });
    }

    // Used to avoid duplicate checks
    std::unordered_set<uint64_t> checkedPairs;
    checkedPairs.reserve(n * 8);

    // Store indices for resolution pass to avoid re-finding colliders
    std::vector<std::pair<size_t, size_t>> collisionIndices;
    collisionIndices.reserve(n);

    for (const auto& [cell, indices] : grid)
    {
        for (size_t aIdx = 0; aIdx < indices.size(); ++aIdx)
        {
            for (size_t bIdx = aIdx + 1; bIdx < indices.size(); ++bIdx)
            {
                size_t i = indices[aIdx];
                size_t j = indices[bIdx];
                if (i == j)
                {
                    continue;
                }

                uint64_t key = packPairKey(i, j);
                if (!checkedPairs.insert(key).second)
                {
                    continue;
                }

                Entity entityA = colliders.entities[i];
                Entity entityB = colliders.entities[j];
                BoxCollider& colliderA = colliders.components[i];
                BoxCollider& colliderB = colliders.components[j];
                SDL_FRect& rectA = rects[i];
                SDL_FRect& rectB = rects[j];

                if (!shouldCheckCollision(colliderA, colliderB))
                {
                    continue;
                }
                // Skip static-static pairs unless a trigger is involved
                if (colliderA.isStatic && colliderB.isStatic && !(colliderA.isTrigger || colliderB.isTrigger))
                {
                    continue;
                }
                if (!intersects(rectA, rectB))
                {
                    continue;
                }

                CollisionEvent event = createCollisionEvent(entityA, entityB, rectA, rectB, colliderA, colliderB);
                outCollisions.push_back(event);
                collisionIndices.emplace_back(i, j);
            }
        }
    }

    if (resolveSolidCollisions)
    {
        for (size_t k = 0; k < outCollisions.size(); ++k)
        {
            const auto [i, j] = collisionIndices[k];
            BoxCollider& colliderA = colliders.components[i];
            BoxCollider& colliderB = colliders.components[j];
            resolveCollision(outCollisions[k], colliderA, colliderB);
        }
    }
}

std::vector<CollisionEvent> collisionSystem(bool resolveSolidCollisions)
{
    std::vector<CollisionEvent> collisions;
    collisionSystem(collisions, resolveSolidCollisions);
    return collisions;
}

