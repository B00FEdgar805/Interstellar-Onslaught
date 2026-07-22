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
#include "../Globals.hpp"
#include <random>

PlayerSystems::PlayerSystems(Entity player)
{
    PLAYER = player;
}

void PlayerSystems::fireSystem(ProjectileSystem projectiles)
{
    
    Uint64 current_time = SDL_GetTicks();
    Transform* player_transform = GLOBALS::REGISTRY.get<Transform>(PLAYER);
    Velocity* player_velocity = GLOBALS::REGISTRY.get<Velocity>(PLAYER);
   // if (current_time - LAST_TIME >= player_projectile -> RATE_OF_FIRE)
    
    if(shoot(current_time - LAST_TIME))
    {
        Vector2D pos = player_transform -> position;
        projectiles.createProjectile(PLAYER, pos + Vector2D(8.0f, 8.0f), player_velocity -> direction.normalize() , 250.0f, DAMAGE);
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

void PlayerSystems::upgradeSpeed(float value)
{
    Velocity* player_velocity = GLOBALS::REGISTRY.get<Velocity>(PLAYER);
    player_velocity -> m_speed *= value;
}

void PlayerSystems::upgradeDamage(float value)
{
    DAMAGE *= value;
}

void PlayerSystems::upgradeHealth(float value)
{
    
    Health* player_health = GLOBALS::REGISTRY.get<Health>(PLAYER);
    player_health -> upgradeHealth(value);
}
 
PlayerSystems::UpgradeType PlayerSystems::randomUpgrade()
 {
     std::random_device rd;
     std::mt19937 gen(rd());
     std::uniform_int_distribution<> distr(1, 4);
     int num = distr(gen);
     switch (num)
     {
         case 1:
             //SDL_Log("R");
             return UPGRADE_ROF;
             break;
         case 2:
             //SDL_Log("S");
             return UPGRADE_SPEED;
             //std::cout << button;
             break;
         case 3:
             //SDL_Log("D");
             return UPGRADE_DAMAGE;
             //std::cout << button;
             break;
         case 4:
             //SDL_Log("H");
             return UPGRADE_HEALTH;
             //std::cout << button;
             break;
         default:
             return UPGRADE_ROF;
             break;
     }
     
     //std::cout << button << " Upgrade" << std::endl;
 }

void PlayerSystems::upgrade(UpgradeType button)
{
    //SDL_Log("Upgrade");
    switch (button)
    {
        case UPGRADE_ROF:
            upgradeROF(1.10f);
            //SDL_Log("R");
            break;
        case UPGRADE_SPEED:
            upgradeSpeed(1.10f);
            //SDL_Log("S");
            break;
        case UPGRADE_DAMAGE:
            upgradeDamage(1.10f);
            //SDL_Log("D");
            break;
        case UPGRADE_HEALTH:
            upgradeHealth(1.10f);
            //SDL_Log("H");
            break;
        default:
            break;
    }
}

std::string PlayerSystems::getLabel(UpgradeType button)
{
    //std::cout << button << std::endl;
    switch (button)
    {
        case UPGRADE_ROF:
            return "ROF";
            break;
        case UPGRADE_SPEED:
            //SDL_Log("Return");
            return "Speed";
            break;
        case UPGRADE_DAMAGE:
            //SDL_Log("Return");
            return "Damage";
            break;
        case UPGRADE_HEALTH:
            //SDL_Log("Return");
            return "Health";
            break;
        default:
            return "";
            break;
    }
}

