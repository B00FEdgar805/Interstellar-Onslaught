#include "ProjectileSystem.hpp"
#include "Components/BoxCollider.hpp"
#include "Components/Sprite.hpp"
#include "Components/Transform.hpp"
#include "Components/Velocity.hpp"
#include "Components/Sprite.hpp"
#include "Components/Health.hpp"
#include "Components/Transform.hpp"
#include "XPSystem.hpp"
#include "../Globals.hpp"

#include <vector>

void ProjectileSystem::projectileSystem(float delta_time)
{
    //std::vector<Entity> deadEntities;
    
    auto view = GLOBALS::REGISTRY.all<Projectile>();
    for (size_t i = 0; i < view.entities.size(); ++i)
    {
        Entity entity = view.entities[i];
        Projectile& projectile = view.components[i];

        projectile.addDistance(delta_time); // Keeps track of distance of projectile
        Velocity* velocity = GLOBALS::REGISTRY.get<Velocity>(entity);    //Makes sure to keep direction of projectile each update
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

Entity ProjectileSystem::createProjectile(Entity owner, const Vector2D& position, const Vector2D& direction, float speed, float damage)
{
    Entity projectile = GLOBALS::REGISTRY.create();
    //DIRECTION = direction;
    
    GLOBALS::REGISTRY.add(projectile, Transform(position));
    GLOBALS::REGISTRY.add(projectile, Velocity(direction, speed));
    GLOBALS::REGISTRY.add(projectile, Projectile(500.0f, speed, damage, owner));
    GLOBALS::REGISTRY.add(projectile, BoxCollider(Vector2D(16.0f, 16.0f), Vector2D(0.0f, 0.0f), true, false, "pProjectile"));
    GLOBALS::REGISTRY.add(projectile, Sprite("Projectile", Vector2D(16.0f, 16.0f)));
    
    //SDL_Log("created projectile");
    Velocity* velocity = GLOBALS::REGISTRY.get<Velocity>(projectile);
    velocity -> direction = direction;
    velocity -> value = direction;
    
    return projectile;
}

void ProjectileSystem::projectilesCollisons(std::vector<CollisionEvent> &collisons)
{
    
    //std::vector<Entity> destroyQueue;
    
    for(const CollisionEvent& collision : collisons)
    {
        Projectile* projectileA = GLOBALS::REGISTRY.get<Projectile>(collision.a);
        Projectile* projectileB = GLOBALS::REGISTRY.get<Projectile>(collision.b);
        
        BoxCollider* a = GLOBALS::REGISTRY.get<BoxCollider>(collision.a);
        BoxCollider* b = GLOBALS::REGISTRY.get<BoxCollider>(collision.b);
        
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
            Health* enemy_health = GLOBALS::REGISTRY.get<Health>(otherEntity);
            //Projectile* p = GLOBALS::REGISTRY.get<Projectile>(projectileEntity);
            enemy_health -> takeDamage(projectile -> getDamage());
            if (!enemy_health -> isAlive())
            {
                Transform* enemy_position = GLOBALS::REGISTRY.get<Transform>(otherEntity);
                deadEntities().push_back(otherEntity);
                XPSystem::spawnXPDrop(enemy_position -> position);
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
