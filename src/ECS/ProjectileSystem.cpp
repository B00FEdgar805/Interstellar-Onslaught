#include "ProjectileSystem.hpp"
#include "Components/BoxCollider.hpp"
#include "Components/Sprite.hpp"
#include "Components/Transform.hpp"
#include "Components/Velocity.hpp"
#include "Components/Sprite.hpp"
#include "Components/Animation.hpp"
#include "Components/Health.hpp"
#include "Components/Transform.hpp"
#include "../AudioManager.hpp"
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

Entity ProjectileSystem::createProjectile(Entity owner, const Vector2D& position, const Vector2D& direction, float speed, float damage, int type)
{
    AudioManager::getInstance().playSound("Shoot", 0.5f);
    Entity projectile = GLOBALS::REGISTRY.create();
    //DIRECTION = direction;
    
    GLOBALS::REGISTRY.add(projectile, Transform(position));
    GLOBALS::REGISTRY.add(projectile, Velocity(direction, speed));
    if(type == 1)   // Shotgun
    {
        GLOBALS::REGISTRY.add(projectile, Projectile(300.0f, speed, damage, owner));
    }
    else
    {
        GLOBALS::REGISTRY.add(projectile, Projectile(600.0f, speed, damage, owner));
    }
    
    if (type == 3) // Railgun
    {
        GLOBALS::REGISTRY.add(projectile, BoxCollider(Vector2D(32.0f, 18.0f), Vector2D(0.0f, 7.0f), true, false, "Railgun"));
        GLOBALS::REGISTRY.add(projectile, Sprite("Railgun", Vector2D(32.0f, 32.0f)));

    }
    else
    {
        GLOBALS::REGISTRY.add(projectile, BoxCollider(Vector2D(16.0f, 16.0f), Vector2D(0.0f, 0.0f), true, false, "pProjectile"));
        GLOBALS::REGISTRY.add(projectile, Sprite("Projectile", Vector2D(16.0f, 16.0f)));
    }
    
    //SDL_Log("created projectile");
    Velocity* velocity = GLOBALS::REGISTRY.get<Velocity>(projectile);
    velocity -> direction = direction;
    velocity -> value = direction;
    
    return projectile;
}

void ProjectileSystem::projectilesCollisons(std::vector<CollisionEvent> &collisons, float delta_time)
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
        else if (((a -> tag == "pProjectile" && b -> tag == "xp") || (a -> tag == "xp" && b -> tag == "pProjectile")))  // to stop collision with xp destroying projectile
        {
            continue;
        }
        
        else if (((a -> tag == "Railgun" && b -> tag == "xp") || (a -> tag == "xp" && b -> tag == "Railgun")))  // to stop collision with xp destroying projectile
        {
            continue;
        }
        
        else if (((a -> tag == "pProjectile" && b -> tag == "pProjectile") || (a -> tag == "pProjectile" && b -> tag == "pProjectile")))  // To stop projectiles from destroying each other
        {
            continue;
        }
        
        else if (((a -> tag == "pProjectile" && b -> tag == "Astroid") || (a -> tag == "Astroid" && b -> tag == "pProjectile")))  // To stop projectiles from destroying each other
        {
            continue;
        }

        else if (((a -> tag == "pProjectile" && b -> tag == "enemy") || (a -> tag == "enemy" && b -> tag == "pProjectile")))
        {
            
            // do damage to enemy
            //SDL_Log("hit");
            Health* enemy_health = GLOBALS::REGISTRY.get<Health>(otherEntity);
            //Projectile* p = GLOBALS::REGISTRY.get<Projectile>(projectileEntity);
            enemy_health -> takeDamage(projectile -> getDamage());
            AudioManager::getInstance().playSound("EnemyHit");
            if (!enemy_health -> isAlive())
            {
                AudioManager::getInstance().playSound("EnemyDeath", 0.5f);
                Transform* enemy_position = GLOBALS::REGISTRY.get<Transform>(otherEntity);
                deadEntities().push_back(otherEntity);
                XPSystem xp;
                xp.spawnXPDrop(enemy_position -> position);
                
                Entity e = GLOBALS::REGISTRY.create();
                GLOBALS::REGISTRY.add(e, Transform(enemy_position -> position));
                GLOBALS::REGISTRY.add(e, Sprite("Explosion"));
                auto s = GLOBALS::REGISTRY.get<Sprite>(e);
                s -> setLoop(false);
                GLOBALS::REGISTRY.add(e, Animation(delta_time, 10, 170));
            }
            
            deadEntities().push_back(projectileEntity);
             
        }
         
        else if (((a -> tag == "Railgun" && b -> tag == "enemy") || (a -> tag == "enemy" && b -> tag == "Railgun")))
        {
            
            // do damage to enemy
            //SDL_Log("hit");
            Health* enemy_health = GLOBALS::REGISTRY.get<Health>(otherEntity);
            //Projectile* p = GLOBALS::REGISTRY.get<Projectile>(projectileEntity);
            enemy_health -> takeDamage(projectile -> getDamage());
            AudioManager::getInstance().playSound("EnemyHit");
            if (!enemy_health -> isAlive())
            {
                AudioManager::getInstance().playSound("EnemyDeath", 0.5f);
                Transform* enemy_position = GLOBALS::REGISTRY.get<Transform>(otherEntity);
                deadEntities().push_back(otherEntity);
                XPSystem xp;
                xp.spawnXPDrop(enemy_position -> position);

                Entity e = GLOBALS::REGISTRY.create();
                GLOBALS::REGISTRY.add(e, Transform(enemy_position -> position));
                GLOBALS::REGISTRY.add(e, Sprite("Explosion"));
                auto s = GLOBALS::REGISTRY.get<Sprite>(e);
                s -> setLoop(false);
                GLOBALS::REGISTRY.add(e, Animation(delta_time, 10, 170));
            }
            //deadEntities().push_back(projectileEntity);
        }
        
        
        
        
        else
        {
            deadEntities().push_back(projectileEntity);
        }
        
            

    }
}
