#include "CollisionSystem.hpp"

#include "Components/BoxCollider.hpp"
#include "Components/Transform.hpp"
#include "Components/Velocity.hpp"

#include <SDL3/SDL.h>

#include <algorithm>
#include <iterator>

#include <unordered_map>
#include <set>
#include <utility>

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

    void moveEntity(Registry& registry, Entity entity, const Vector2D& amount)
    {
        Transform* transform = registry.get<Transform>(entity);

        if (transform == nullptr)
        {
            return;
        }

        transform -> position.x += amount.x;
        transform -> position.y += amount.y;
    }

    void stopVelocityOnCollisionAxis(Registry& registry, Entity entity, const Vector2D& normal)
    {
        Velocity* velocity = registry.get<Velocity>(entity);

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

    void resolveCollision(Registry& registry, const CollisionEvent& event, const BoxCollider& colliderA, const BoxCollider& colliderB)
    {
        if (colliderA.tag == "enemy" && colliderB.tag == "enemy")
        {
            return;
        }
        
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
            moveEntity(registry, event.a, correctionA);
            stopVelocityOnCollisionAxis(registry, event.a, event.normal);
            return;
        }

        if (colliderA.isStatic && !colliderB.isStatic)
        {
            moveEntity(registry, event.b, correctionB);
            stopVelocityOnCollisionAxis(registry, event.b, event.normal);
            return;
        }

        Vector2D halfCorrectionA(correctionA.x * 0.5f,correctionA.y * 0.5f);

        Vector2D halfCorrectionB(correctionB.x * 0.5f, correctionB.y * 0.5f);

        moveEntity(registry, event.a, halfCorrectionA);
        moveEntity(registry, event.b, halfCorrectionB);

        stopVelocityOnCollisionAxis(registry, event.a, event.normal);
        stopVelocityOnCollisionAxis(registry, event.b, event.normal);
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
    static std::vector<GridCell> getCellsForRect(const SDL_FRect& rect)
{
        std::vector<GridCell> cells;
        int x0 = (int)std::floor(rect.x / GRID_CELL_SIZE);
        int y0 = (int)std::floor(rect.y / GRID_CELL_SIZE);
        int x1 = (int)std::floor((rect.x + rect.w) / GRID_CELL_SIZE);
        int y1 = (int)std::floor((rect.y + rect.h) / GRID_CELL_SIZE);
        for (int x = x0; x <= x1; ++x)
        {
            for (int y = y0; y <= y1; ++y)
            {
                cells.push_back({x, y});
            }
        }
        return cells;
    }
}

std::vector<CollisionEvent> collisionSystem(Registry& registry, bool resolveSolidCollisions)
{
    std::vector<CollisionEvent> collisions;
    auto colliders = registry.all<BoxCollider>();
    const size_t n = colliders.entities.size();
    if (n <= 1) return collisions;

    // --- Spatial Hashing ---
    std::unordered_map<GridCell, std::vector<size_t>> grid; // GridCell indices in colliders array
    std::vector<SDL_FRect> rects(n);
    for (size_t i = 0; i < n; ++i) {
        Entity entity = colliders.entities[i];
        BoxCollider& collider = colliders.components[i];
        Transform* transform = registry.get<Transform>(entity);
        if (!transform) continue;
        rects[i] = makeWorldRect(*transform, collider);
        for (const auto& cell : getCellsForRect(rects[i])) {
            grid[cell].push_back(i);
        }
    }
    // Used to avoid duplicate checks
    std::set<std::pair<size_t, size_t>> checkedPairs;

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
                
                size_t minIdx = std::min(i, j), maxIdx = std::max(i, j);
                auto pair = std::make_pair(minIdx, maxIdx);
                if (checkedPairs.count(pair))
                {
                    continue;
                }
                
                checkedPairs.insert(pair);

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
                if (!intersects(rectA, rectB))
                {
                    continue;
                }
                
                CollisionEvent event = createCollisionEvent(entityA, entityB, rectA, rectB, colliderA, colliderB);
                
                collisions.push_back(event);
                
                if (resolveSolidCollisions)
                {
                    resolveCollision(registry, event, colliderA, colliderB);
                }
            }
        }
    }
    return collisions;
}
