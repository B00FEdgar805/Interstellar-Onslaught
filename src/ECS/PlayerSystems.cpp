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
#include "Components/BoxCollider.hpp"
#include "Components/Sprite.hpp"
#include "XPSystem.hpp"
#include "../Globals.hpp"
#include <random>

PlayerSystems::PlayerSystems(Entity player)
{
    PLAYER = player;
    ASTROID_L = GLOBALS::REGISTRY.create();
    GLOBALS::REGISTRY.add(ASTROID_L, Sprite("Astroid", Vector2D(16.0f, 16.0f)));
    GLOBALS::REGISTRY.add(ASTROID_L, Transform(Vector2D(100.0f, 100.0f)));
    GLOBALS::REGISTRY.add(ASTROID_L, BoxCollider(Vector2D(16.0f, 16.0f), Vector2D(0.0f, 0.0f), false, false, "Astroid"));
    ASTROID_R = GLOBALS::REGISTRY.create();
    GLOBALS::REGISTRY.add(ASTROID_R, Sprite("Astroid", Vector2D(16.0f, 16.0f)));
    GLOBALS::REGISTRY.add(ASTROID_R, Transform(Vector2D(100.0f, 100.0f)));
    GLOBALS::REGISTRY.add(ASTROID_R, BoxCollider(Vector2D(16.0f, 16.0f), Vector2D(0.0f, 0.0f), false, false, "Astroid"));
    HAS_ASTROID = true;
}

void PlayerSystems::fireSystem(ProjectileSystem projectiles, float delta)
{
    
    Uint64 current_time = SDL_GetTicks();
    Uint64 start_time = SDL_GetTicks();

    Transform* player_transform = GLOBALS::REGISTRY.get<Transform>(PLAYER);
    Velocity* player_velocity = GLOBALS::REGISTRY.get<Velocity>(PLAYER);
   // if (current_time - LAST_TIME >= player_projectile -> RATE_OF_FIRE)
    switch (CURRENT_WEAPON)
    {
        case WEAPON_NORMAL:
            if(shoot(current_time - LAST_TIME))
            {
                Vector2D pos = player_transform -> position;
                projectiles.createProjectile(PLAYER, pos + Vector2D(8.0f, 8.0f), player_velocity -> direction.normalize() , PROJECTILE_SPEED, DAMAGE, CURRENT_WEAPON);
                LAST_TIME = current_time;
            }
            break;
        case WEAPON_SHOTGUN:
            if(shoot(current_time - LAST_TIME))
            {
                Vector2D pos = player_transform -> position;
                Vector2D mainDir = player_velocity -> direction.normalize();
                float angle = 20.0f * (M_PI / 180.0f); // Convert degrees to radians

                // Left (+45 degrees)
                float cosA = cos(angle);
                float sinA = sin(angle);
                Vector2D dirL(mainDir.x * cosA - mainDir.y * sinA, mainDir.x * sinA + mainDir.y * cosA);

                // Right (-45 degrees)
                float cosA_r = cos(-angle);
                float sinA_r = sin(-angle);
                Vector2D dirR(mainDir.x * cosA_r - mainDir.y * sinA_r, mainDir.x * sinA_r + mainDir.y * cosA_r);
                
                projectiles.createProjectile(PLAYER, pos + Vector2D(8.0f, 8.0f), player_velocity -> direction.normalize() , PROJECTILE_SPEED * 0.8f, DAMAGE, CURRENT_WEAPON);
                projectiles.createProjectile(PLAYER, pos + Vector2D(8.0f, 8.0f), dirL , PROJECTILE_SPEED * 0.8f, DAMAGE, CURRENT_WEAPON);
                projectiles.createProjectile(PLAYER, pos + Vector2D(8.0f, 8.0f), dirR , PROJECTILE_SPEED * 0.8f, DAMAGE, CURRENT_WEAPON);
                LAST_TIME = current_time;
            }
            break;
        case WEAPON_SMG:
            if(shoot(current_time - LAST_TIME))
            {
                Vector2D pos = player_transform -> position;
                projectiles.createProjectile(PLAYER, pos + Vector2D(8.0f, 8.0f), player_velocity -> direction.normalize() , PROJECTILE_SPEED, DAMAGE * 0.5, CURRENT_WEAPON);
                LAST_TIME = current_time;
            }
            break;
        case WEAPON_RAILGUN:
            if(shoot(current_time - LAST_TIME))
            {
                Vector2D pos = player_transform -> position;
                projectiles.createProjectile(PLAYER, pos + Vector2D(0.0f, 0.0f), player_velocity -> direction.normalize() , PROJECTILE_SPEED * 2.0f, DAMAGE * 2.0f, CURRENT_WEAPON);
                LAST_TIME = current_time;
            }
            break;
    }
    
    if (HAS_SHEILD)
    {
        if(!GLOBALS::INVINCIBLE && ((start_time - LAST_TIME_SHIELD) >= SHIELD_TIME))
        {
            GLOBALS::INVINCIBLE = true;
           // SDL_Log("Shield");
            LAST_TIME_SHIELD = start_time;
        }
    }
    
    if (HAS_ASTROID)
    {
        //Velocity* v = GLOBALS::REGISTRY.get<Velocity>(ASTROID_L);

        
        float orbitX = player_transform -> position.x + 16.0f;
        float orbitY = player_transform -> position.y + 16.0f;
        float radius = 64.0f;
        
        angle += 3.0f * delta;
        if (angle >= 2.0f * M_PI) angle -= 2.0f * M_PI;

        float x = orbitX + cosf(angle) * radius;
        float y = orbitY + sinf(angle) * radius;
        
        Vector2D posR = {x,y};
        float xl = 2 * orbitX - x;
        float yl = 2 * orbitY - y;
        Vector2D posL = {xl,yl};
        //posL.scale(-1);
        
        Transform* positionL = GLOBALS::REGISTRY.get<Transform>(ASTROID_L);
        Transform* positionR = GLOBALS::REGISTRY.get<Transform>(ASTROID_R);
        
        if (!positionL)
        {
            SDL_Log("left is null");
        }
        
        if (!positionR)
        {
            SDL_Log("right is null");
        }
        //std::cout << pos;
        //v -> value = pos;
        
        positionL -> position = posL;
        //std::cout << "Left" << posL;
        positionR -> position = posR;
        //std::cout << "Right: " << posR;


    }
    
}

bool PlayerSystems::shoot(float time)
{
    switch (CURRENT_WEAPON)
    {
        case WEAPON_NORMAL:
            if(time >= RATE_OF_FIRE)
            {
                return true;
            }
            else
            {
                return false;
            }
            break;
        case WEAPON_SHOTGUN:
            if(time >= (RATE_OF_FIRE * 1.30f))
            {
                return true;
            }
            else
            {
                return false;
            }
            break;
        case WEAPON_SMG:
            if(time >= RATE_OF_FIRE * 0.2f)
            {
                return true;
            }
            else
            {
                return false;
            }
            break;
        case WEAPON_RAILGUN:
            if(time >= (RATE_OF_FIRE * 4.0f))
            {
                return true;
            }
            else
            {
                return false;
            }
            break;
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
 
void PlayerSystems::upgradeXPMutiplier(float value)
{
    XPSystem::XP_MULTIPLIER *= value;
}

void PlayerSystems::upgradeProjectileSpeed(float value)
{
    PROJECTILE_SPEED *= value;
}

void PlayerSystems::upgradeXPGrabRange(float value)
{
    XPSystem::XP_GRAB_RANGE *= value;
}

PlayerSystems::UpgradeType PlayerSystems::randomUpgrade()
 {
     std::random_device rd;
     std::mt19937 gen(rd());
     std::uniform_int_distribution<> distr(1, 9);
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
         case 5:
             //SDL_Log("R");
             return UPGRADE_PROJECTILE_SPEED;
             break;
         case 6:
             //SDL_Log("R");
             return UPGRADE_XP_MUTIPLIER;
             break;
         case 7:
             //SDL_Log("R");
             return UPGRADE_XP_RANGE;
             break;
         case 8:
             //SDL_Log("Weapon");
             return randomUpgradeWeapon();
             break;
         case 9:
             return randomUpgradeUnique();
             break;
         default:
             return UPGRADE_ROF;
             break;
     }
     
     //std::cout << button << " Upgrade" << std::endl;
}


// need to redo this function for unique upgrades
PlayerSystems::UpgradeType PlayerSystems::randomUpgradeUnique()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(1, 3);
    int num = distr(gen);
    switch (num)
    {
        case 1:
            //SDL_Log("R");
            return UPGRADE_SHEILD;
            break;
        case 2:
            //SDL_Log("S");
            return UPGRADE_ASTROIDS;
            //std::cout << button;
            break;
        case 3:
            //SDL_Log("D");
            return UPGRADE_GUNNER;
            //std::cout << button;
            break;
        default:
            return UPGRADE_ROF;
    }
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
        case UPGRADE_PROJECTILE_SPEED:
            upgradeProjectileSpeed(1.10f);
            break;
        case UPGRADE_XP_MUTIPLIER:
            upgradeXPMutiplier(1.10f);
            break;
        case UPGRADE_XP_RANGE:
            upgradeXPGrabRange(1.20f);
            break;
        case UPGRADE_WEAPON_NORMAL:
            CURRENT_WEAPON = WEAPON_NORMAL;
            upgradeROF(1.10f);
            upgradeDamage(1.10f);
            upgradeProjectileSpeed(1.10f);
            break;
        case UPGRADE_WEAPON_SHOTGUN:
            CURRENT_WEAPON = WEAPON_SHOTGUN;
            upgradeROF(1.10f);
            upgradeDamage(1.10f);
            upgradeProjectileSpeed(1.10f);
            break;
        case UPGRADE_WEAPON_SMG:
            CURRENT_WEAPON = WEAPON_SMG;
            upgradeROF(1.10f);
            upgradeDamage(1.10f);
            upgradeProjectileSpeed(1.10f);
            break;
        case UPGRADE_WEAPON_RAILGUN:
            CURRENT_WEAPON = WEAPON_RAILGUN;
            upgradeROF(1.10f);
            upgradeDamage(1.10f);
            upgradeProjectileSpeed(1.10f);
            break;
        case UPGRADE_SHEILD:
            if (HAS_SHEILD)
            {
                SHIELD_TIME /= 1.30f;
            }
            else
            {
                HAS_SHEILD = true;
                // do function
            }
            break;
        case UPGRADE_ASTROIDS:
            if (HAS_ASTROID)
            {
                upgradeDamage(1.20f);
            }
            else
            {
                /*
                GLOBALS::REGISTRY.add(ASTROID_L, Sprite("Astroid", Vector2D(16.0f, 16.0f)));
                GLOBALS::REGISTRY.add(ASTROID_L, Transform(Vector2D(100.0f, 100.0f)));
                //GLOBALS::REGISTRY.add(ASTROID_L, Velocity(150.0f));
                GLOBALS::REGISTRY.add(ASTROID_L, BoxCollider(Vector2D(16.0f, 16.0f), Vector2D(0.0f, 0.0f), false, false, "Astroid"));
                GLOBALS::REGISTRY.add(ASTROID_R, Sprite("Astroid", Vector2D(16.0f, 16.0f)));
                GLOBALS::REGISTRY.add(ASTROID_R, Transform(Vector2D(100.0f, 100.0f)));
               // GLOBALS::REGISTRY.add(ASTROID_R, Velocity(150.0f));
                GLOBALS::REGISTRY.add(ASTROID_R, BoxCollider(Vector2D(16.0f, 16.0f), Vector2D(0.0f, 0.0f), false, false, "Astroid"));
                */
                 HAS_ASTROID = true;
                // do function
                 
            }
            break;
        case UPGRADE_GUNNER:
            if (HAS_GUNNER == 0)
            {
                // spawn left
                HAS_GUNNER++;
            }
            else if (HAS_GUNNER == 1)
            {
                // spawn right
                HAS_GUNNER++;
            }
            else
            {
                upgradeDamage(1.20f);
            }
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
        case UPGRADE_PROJECTILE_SPEED:
            return "PSpeed";
            break;
        case UPGRADE_XP_MUTIPLIER:
            return "XPMutiplier";
            break;
        case UPGRADE_XP_RANGE:
            return "XPRange";
            break;
        case UPGRADE_WEAPON_NORMAL:
            return "Normal";
            break;
        case UPGRADE_WEAPON_SHOTGUN:
            return "Shotgun";
            break;
        case UPGRADE_WEAPON_SMG:
            return "SMG";
            break;
        case UPGRADE_WEAPON_RAILGUN:
            return "Railgun";
            break;
        case UPGRADE_SHEILD:
            return "Shield";
            break;
        case UPGRADE_ASTROIDS:
            return "Astroids";
            break;
        case UPGRADE_GUNNER:
            return "Gunner";
            break;
        default:
            return "";
            break;
    }
}

PlayerSystems::UpgradeType PlayerSystems::randomUpgradeWeapon()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(1, 4);
    int num = distr(gen);
    switch (num)
    {
        case 1:
            return UPGRADE_WEAPON_NORMAL;
            break;
        case 2:
            return UPGRADE_WEAPON_SHOTGUN;
            break;
        case 3:
            return UPGRADE_WEAPON_SMG;
            break;
        case 4:
            return UPGRADE_WEAPON_RAILGUN;
            break;
        default:
            return UPGRADE_WEAPON_NORMAL;
            break;
    }
}
