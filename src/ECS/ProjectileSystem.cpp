#include "ProjectileSystem.hpp"
#include "Components/BoxCollider.hpp"
#include "Components/Sprite.hpp"
#include "Components/Transform.hpp"
#include "Components/Velocity.hpp"
#include "Components/Sprite.hpp"

#include <vector>

void ProjectileSystem::projectileSystem(Registry& registry, float delta_time)
{
    std::vector<Entity> deadEntities;
    
    for(auto& [entity, projectile] : registry.all<Projectile>())
    {
        projectile.addDistance(delta_time); // Keeps track of distance of projectile
        Velocity* velocity = registry.get<Velocity>(entity);    //Makes sure to keep direction of projectile each update
        velocity -> value = Vector2D(0.0f, 0.0f);
        velocity -> value += DIRECTION;
        
        if (projectile.hasExpired())
        {
            //SDL_Log("Dead");
            deadEntities.push_back(entity);
        }
        
    }
    
    for(Entity entity : deadEntities)   // Destroy all projectiles that have expired
    {
        registry.destroy(entity);
    }
}

Entity ProjectileSystem::createProjectile(Registry& registry, Entity owner, const Vector2D& position, const Vector2D& direction, float speed)
{
    Entity projectile = registry.create();
    DIRECTION = direction;
    
    registry.add(projectile, Transform(position));
    registry.add(projectile, Velocity(direction, speed));
    registry.add(projectile, Projectile(500.0f, speed, RATE_OF_FIRE));
    registry.add(projectile, BoxCollider(Vector2D(16.0f, 16.0f), Vector2D(0.0f, 0.0f), true, false, "pProjectile"));
    registry.add(projectile, Sprite("Projectile", Vector2D(16.0f, 16.0f)));
    
    //SDL_Log("created projectile");
    
    
    return projectile;
}

void ProjectileSystem::projectilesCollisons(Registry &registry, std::vector<CollisionEvent> &collisons)
{
    std::vector<Entity> destroyQueue;
    
    for(const CollisionEvent& collision : collisons)
    {
        Projectile* projectileA = registry.get<Projectile>(collision.a);
        Projectile* projectileB = registry.get<Projectile>(collision.b);
        
        Entity projectileEntity = 0;
        Entity otherEntity = 0; // Any entity thats is not a projectile
        Projectile* projectile = nullptr;
        
        if (projectileA)
        {
            projectileEntity = collision.a;
            otherEntity = collision.b;
            projectile = projectileA;
        }
        else if(projectileB)
        {
            projectileEntity = collision.b;
            otherEntity = collision.a;
            projectile = projectileB;
        }
        else
        {
            continue;
        }
        
       
        
        destroyQueue.push_back(projectileEntity);
        
        for(Entity entity: destroyQueue)    // Destroys any projectiles that have had collsions
        {
            registry.destroy(entity);
        }

    }
}
