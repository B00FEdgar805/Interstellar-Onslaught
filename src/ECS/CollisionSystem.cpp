#include "CollisionSystem.hpp"

#include "Components/BoxCollider.hpp"
#include "Components/Transform.hpp"
#include "Components/Velocity.hpp"

#include <SDL3/SDL.h>

#include <algorithm>
#include <iterator>

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
}

std::vector<CollisionEvent> collisionSystem(Registry& registry, bool resolveSolidCollisions)
{
    std::vector<CollisionEvent> collisions;

    auto& colliders = registry.all<BoxCollider>();

    for (auto itA = colliders.begin(); itA != colliders.end(); ++itA)
    {
        Entity entityA = itA -> first;
        BoxCollider& colliderA = itA -> second;

        Transform* transformA = registry.get<Transform>(entityA);

        if (transformA == nullptr)
        {
            continue;
        }

        for (auto itB = std::next(itA); itB != colliders.end(); ++itB)
        {
            Entity entityB = itB -> first;
            BoxCollider& colliderB = itB -> second;

            Transform* transformB = registry.get<Transform>(entityB);

            if (transformB == nullptr)
            {
                continue;
            }

            if (!shouldCheckCollision(colliderA, colliderB))
            {
                continue;
            }

            SDL_FRect rectA = makeWorldRect(*transformA, colliderA);
            SDL_FRect rectB = makeWorldRect(*transformB, colliderB);

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

    return collisions;
}
