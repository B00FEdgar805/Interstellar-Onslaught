//
//  PlayerSystems.cpp
//  GameTestSDL3
//
//  Created by Edgar Alamillo on 7/8/26.
//

#include "PlayerSystems.hpp"
#include "Components/Transform.hpp"
#include "Components/Velocity.hpp"
#include "Components/Health.hpp"

PlayerSystems::PlayerSystems(Entity player)
{
    PLAYER = player;
}

void PlayerSystems::fireSystem(ProjectileSystem projectiles,  Registry& REGISTRY)
{
    
    Uint64 current_time = SDL_GetTicks();
    Transform* player_transform = REGISTRY.get<Transform>(PLAYER);
    Velocity* player_velocity = REGISTRY.get<Velocity>(PLAYER);
   // if (current_time - LAST_TIME >= player_projectile -> RATE_OF_FIRE)
    
    if(shoot(current_time - LAST_TIME))
    {
        Vector2D pos = player_transform -> position;
        projectiles.createProjectile(REGISTRY, PLAYER, pos + Vector2D(8.0f, 8.0f), player_velocity -> direction.normalize() , 250.0f, DAMAGE);
        LAST_TIME = current_time;
    }
    
}

bool PlayerSystems::shoot(float time)
{
    if(time >= RATE_OF_FIRE)
    {
        return true;
    }
    else
    {
        return false;
    }
}


void PlayerSystems::upgradeROF(float value)
{
    RATE_OF_FIRE /= value;
}

void PlayerSystems::upgradeSpeed(float value, Registry &REGISTRY)
{
    Velocity* player_velocity = REGISTRY.get<Velocity>(PLAYER);
    player_velocity -> m_speed *= value;
}

void PlayerSystems::upgradeDamage(float value)
{
    DAMAGE *= value;
}

void PlayerSystems::upgradeHealth(float value, Registry &REGISTRY)
{
    Health* player_health = REGISTRY.get<Health>(PLAYER);
    player_health -> upgradeHealth(value);
}
