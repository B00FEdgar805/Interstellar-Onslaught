#include "ProjectileSystem.hpp"
#include "Components/BoxCollider.hpp"
#include "Components/Sprite.hpp"
#include "Components/Transform.hpp"
#include "Components/Velocity.hpp"
#include "Components/Sprite.hpp"
#include "Components/Health.hpp"
#include "Components/Transform.hpp"
#include "XPSystem.hpp"

#include <vector>

void ProjectileSystem::projectileSystem(Registry& registry, float delta_time)
{
    //std::vector<Entity> deadEntities;
    
    auto view = registry.all<Projectile>();
    for (size_t i = 0; i < view.entities.size(); ++i)
    {
        Entity entity = view.entities[i];
        Projectile& projectile = view.components[i];

        projectile.addDistance(delta_time); // Keeps track of distance of projectile
        Velocity* velocity = registry.get<Velocity>(entity);    //Makes sure to keep direction of projectile each update
        velocity -> value.zero();
        velocity -> value += velocity -> direction.normalize();
        //SDL_Log("%f", velocity -> directionToDegrees());
        if (projectile.hasExpired())
        {
            //SDL_Log("Projectile Dead");
            deadEntities().push_back(entity);
        }
    }
}

Entity ProjectileSystem::createProjectile(Registry& registry, Entity owner, const Vector2D& position, const Vector2D& direction, float speed, float damage)
{
    Entity projectile = registry.create();
    //DIRECTION = direction;
    
    registry.add(projectile, Transform(position));
    registry.add(projectile, Velocity(direction, speed));
    registry.add(projectile, Projectile(500.0f, speed, damage, owner));
    registry.add(projectile, BoxCollider(Vector2D(16.0f, 16.0f), Vector2D(0.0f, 0.0f), true, false, "pProjectile"));
    registry.add(projectile, Sprite("Projectile", Vector2D(16.0f, 16.0f)));
    
    //SDL_Log("created projectile");
    Velocity* velocity = registry.get<Velocity>(projectile);
    velocity -> direction = direction;
    velocity -> value = direction;
    
    return projectile;
}

void ProjectileSystem::projectilesCollisons(Registry &registry, std::vector<CollisionEvent> &collisons)
{
    
    //std::vector<Entity> destroyQueue;
    
    for(const CollisionEvent& collision : collisons)
    {
        Projectile* projectileA = registry.get<Projectile>(collision.a);
        Projectile* projectileB = registry.get<Projectile>(collision.b);
        
        BoxCollider* a = registry.get<BoxCollider>(collision.a);
        BoxCollider* b = registry.get<BoxCollider>(collision.b);
        
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
        
        if (otherEntity == projectile -> OWNER)
        {
            //SDL_Log("hit self");
            continue;
        }
        
        else if (((a -> tag == "pProjectile" && b -> tag == "enemy") || (a -> tag == "enemy" && b -> tag == "pProjectile")))
        {
            
            // do damage to enemy
            //SDL_Log("hit");
            Health* enemy_health = registry.get<Health>(otherEntity);
            //Projectile* p = registry.get<Projectile>(projectileEntity);
            enemy_health -> takeDamage(projectile -> getDamage());
            if (!enemy_health -> isAlive())
            {
                Transform* enemy_position = registry.get<Transform>(otherEntity);
                deadEntities().push_back(otherEntity);
                XPSystem::spawnXPDrop(registry, enemy_position -> position);
                // spawn xp pick up
            }
            deadEntities().push_back(projectileEntity);
             
        }
         
        else
        {
            deadEntities().push_back(projectileEntity);
        }
        
    
        // Add check if it destroy on hit
        

    }
}
