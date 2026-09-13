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
#include "Components/Item.hpp"
#include "Components/Animation.hpp"
#include "XPSystem.hpp"
#include "../Globals.hpp"
#include "../AudioManager.hpp"
#include <random>

PlayerSystems::PlayerSystems(Entity player)
{
    PLAYER = player;
    
    ASTROID_L = GLOBALS::REGISTRY.create();
    GLOBALS::REGISTRY.add(ASTROID_L, Sprite("Astroid", Vector2D(16.0f, 16.0f)));
    GLOBALS::REGISTRY.add(ASTROID_L, Transform(Vector2D(-100.0f, -100.0f)));
    GLOBALS::REGISTRY.add(ASTROID_L, BoxCollider(Vector2D(16.0f, 16.0f), Vector2D(0.0f, 0.0f), false, false, "Astroid"));
    ASTROID_R = GLOBALS::REGISTRY.create();
    GLOBALS::REGISTRY.add(ASTROID_R, Sprite("Astroid", Vector2D(16.0f, 16.0f)));
    GLOBALS::REGISTRY.add(ASTROID_R, Transform(Vector2D(-100.0f, -100.0f)));
    GLOBALS::REGISTRY.add(ASTROID_R, BoxCollider(Vector2D(16.0f, 16.0f), Vector2D(0.0f, 0.0f), false, false, "Astroid"));
}

void PlayerSystems::fireSystem(ProjectileSystem projectiles, float delta)
{
    
    Uint64 current_time = SDL_GetTicks();
    Uint64 start_time = SDL_GetTicks();
    Uint64 start_time_gunner = SDL_GetTicks();

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
                projectiles.createProjectile(PLAYER, pos + Vector2D(0.0f, 0.0f), player_velocity -> direction.normalize() , PROJECTILE_SPEED * 2.0f, DAMAGE * 1.5f, CURRENT_WEAPON);
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
        
        if (GLOBALS::INVINCIBLE)
        {
            Transform* pos = GLOBALS::REGISTRY.get<Transform>(SHIELD);
            pos -> position = player_transform -> position;
        }
        else
        {
            Transform* pos = GLOBALS::REGISTRY.get<Transform>(SHIELD);
            pos -> position = Vector2D(-200.0f, -200.0f);
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
            return;
        }
        
        if (!positionR)
        {
            SDL_Log("right is null");
            return;
        }
        //std::cout << pos;
        //v -> value = pos;
        
        positionL -> position = posL;
        //std::cout << "Left" << posL;
        positionR -> position = posR;
        //std::cout << "Right: " << posR;

    }

    switch (HAS_GUNNER)
    {
        case 0:
            break;
        case 1:
        {
            Transform* pos = GLOBALS::REGISTRY.get<Transform>(GUNNER_L);
            if (!pos)
            {
                SDL_Log("pos is null");
                break;
            }
            /*
            Vector2D mainDir = player_velocity -> direction.normalize();
            Vector2D player_pos = player_transform -> position;
            player_pos += Vector2D(16.0f, 16.0f);
            float leftOffset = 16.0f; // distance to the left of the player
            Vector2D leftDir(-mainDir.y, mainDir.x);
            leftDir.normalize();
            player_pos += leftDir.scale(leftOffset);
            pos -> position = player_pos;
             */
            
            float totalAngleDegrees = player_velocity -> directionToDegrees() + 180.0f;
            float angleRadians = totalAngleDegrees * (M_PI / 180.0f);
            Vector2D player_pos = player_transform -> position;
            player_pos += Vector2D(16.0f, 16.0f);
            
            float orbitCenterX = player_pos.x + (16.0f * cosf(angleRadians));
            float orbitCenterY = player_pos.y + (16.0f * sinf(angleRadians));
            pos -> position = Vector2D(orbitCenterX - 16.0f, orbitCenterY - 16.0f);
            
            if((start_time_gunner - LAST_TIME_GUNNER) >= RATE_OF_FIRE)
            {
                projectiles.createProjectile(PLAYER, pos -> position, player_velocity -> direction.normalize() , PROJECTILE_SPEED, DAMAGE, 0);

                LAST_TIME_GUNNER = start_time_gunner;
            }

        }
            break;
        case 2:
        {
            Transform* posL = GLOBALS::REGISTRY.get<Transform>(GUNNER_L);
            if (!posL)
            {
                SDL_Log("posL is null");
                break;
            }
            Transform* posR = GLOBALS::REGISTRY.get<Transform>(GUNNER_R);
            if (!posR)
            {
                SDL_Log("posR is null");
                break;
            }
            
            /*
            Vector2D mainDir = player_velocity -> direction;
            mainDir.normalize();
            Vector2D player_pos = player_transform -> position;
            player_pos += Vector2D(16.0f, 16.0f);
            float leftOffset = 16.0f; // distance to the left of the player
            float rightOffset = 16.0f; // distance to the right of the player
            Vector2D leftDir(-mainDir.y, mainDir.x);
            Vector2D rightDir(mainDir.y, -mainDir.x);
            leftDir.normalize();
            rightDir.normalize();
            //player_pos += leftDir.scale(leftOffset);
            Vector2D newPosL = player_pos;
            newPosL.add(leftDir.scale(leftOffset));
            
            Vector2D newPosR = player_pos;
            newPosR.add(rightDir.scale(rightOffset));
            
            posL -> position = newPosL;
            posR -> position = newPosR;
             */
            float totalAngleDegreesL = player_velocity -> directionToDegrees() + 180.0f;
            float angleRadiansL = totalAngleDegreesL * (M_PI / 180.0f);
            Vector2D player_pos = player_transform -> position;
            player_pos += Vector2D(16.0f, 16.0f);
            
            float orbitCenterXL = player_pos.x + (16.0f * cosf(angleRadiansL));
            float orbitCenterYL = player_pos.y + (16.0f * sinf(angleRadiansL));
            
            float totalAngleDegreesR = player_velocity -> directionToDegrees();
            float angleRadiansR = totalAngleDegreesR * (M_PI / 180.0f);
            //Vector2D player_pos = player_transform -> position;
            //player_pos += Vector2D(16.0f, 16.0f);
            
            float orbitCenterXR = player_pos.x + (16.0f * cosf(angleRadiansR));
            float orbitCenterYR = player_pos.y + (16.0f * sinf(angleRadiansR));
            
            
            posL -> position = Vector2D(orbitCenterXL - 16.0f, orbitCenterYL - 16.0f);
            posR -> position = Vector2D(orbitCenterXR - 16.0f, orbitCenterYR - 16.0f);
            
            if((start_time_gunner - LAST_TIME_GUNNER) >= RATE_OF_FIRE)
            {
                projectiles.createProjectile(PLAYER, posL -> position, player_velocity -> direction.normalize() , PROJECTILE_SPEED, DAMAGE, 0);
                projectiles.createProjectile(PLAYER, posR -> position, player_velocity -> direction.normalize() , PROJECTILE_SPEED, DAMAGE, 0);

                LAST_TIME_GUNNER = start_time_gunner;
            }
        }
            break;
        default:
            break;
    }
    if (POWER_UP)
    {
        Uint64 current_time_power_up = SDL_GetTicks();
        Uint64 elampsed_power_up = current_time_power_up - POWER_UP_START;
        if (elampsed_power_up > POWER_UP_TIME)
        {
            powerUp();
            POWER_UP = false;
        }
        //std::cout << elampsed_power_up << std::endl;
        
        if (CURRENT_POWER_UP == POWER_UP_GRAB_XP)
        {
            auto view = GLOBALS::REGISTRY.all<Item>();
            for (size_t i = 0; i < view.entities.size(); ++i)
            {
                Entity entity = view.entities[i];
                Transform* pos = GLOBALS::REGISTRY.get<Transform>(entity);
                Vector2D player_pos = player_transform -> position;
                Vector2D pos_player = player_pos - pos -> position;
                Vector2D dir_player = pos_player.normalize();
                float speed = 100.0f; // Set your desired speed
                pos -> position += dir_player.scale(speed * delta);
            }
        }
    }
    
    SPAWN_TIME += delta;
    //SDL_Log("%f", SPAWN_TIME);
    if (SPAWN_TIME >= 60.0f)
    {
        SPAWN_TIME = 0.0f;
        //SDL_Log("Spawned");
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distr(1, 4);
        int num = distr(gen);
        float x = 0.0f;
        float y = 0.0f;
        switch (num)
        {
            case 1:
                // keep x and y 0
                break;
            case 2:
                x = GLOBALS::SCREEN_WIDTH;
                break;
            case 3:
                y = GLOBALS::SCREEN_HEIGHT;
                break;
            case 4:
            {
                x = GLOBALS::SCREEN_WIDTH;
                y = GLOBALS::SCREEN_HEIGHT;
            }
                break;
            default:
                break;
        }
        //x = 100.0f;
        //y = 100.0f;
        
        Entity sattelite = GLOBALS::REGISTRY.create();
        GLOBALS::REGISTRY.add(sattelite, Transform(Vector2D(x,y)));
        GLOBALS::REGISTRY.add(sattelite, Sprite("Sattelite", Vector2D(64.0f, 32.0f)));
        GLOBALS::REGISTRY.add(sattelite, Animation(delta, 2, 150));
        GLOBALS::REGISTRY.add(sattelite, BoxCollider(Vector2D(64.0f, 32.0f), Vector2D(0.0f, 0.0f), true, false, "sattelite"));
        GLOBALS::REGISTRY.add(sattelite, Item());

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

void PlayerSystems::upgradePowerUpTime(float value)
{
    POWER_UP_TIME *= value;
}

void PlayerSystems::upgradeHealAmount(float value)
{
    HEAL_AMOUNT *= value;
}

PlayerSystems::UpgradeType PlayerSystems::randomUpgrade()
 {
     std::random_device rd;
     std::mt19937 gen(rd());
     std::uniform_int_distribution<> distr(1, 11);
     int num = distr(gen);
    //int num = 9;
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
         case 10:
             return UPGRADE_POWER_UP_TIME;
             break;
         case 11:
             return UPGRADE_HEAL_AMOUNT;
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
    Health* health = GLOBALS::REGISTRY.get<Health>(PLAYER);
    //SDL_Log("%f" , health -> getHealth());
    health -> heal(HEAL_AMOUNT);
    //SDL_Log("%f" , health -> getHealth());

    
    switch (button)
    {
        case UPGRADE_ROF:
            upgradeROF(1.30f);
            //SDL_Log("R");
            break;
        case UPGRADE_SPEED:
            upgradeSpeed(1.30f);
            //SDL_Log("S");
            break;
        case UPGRADE_DAMAGE:
            upgradeDamage(1.30f);
            //SDL_Log("D");
            break;
        case UPGRADE_HEALTH:
            upgradeHealth(1.30f);
            //SDL_Log("H");
            break;
        case UPGRADE_PROJECTILE_SPEED:
            upgradeProjectileSpeed(1.30f);
            break;
        case UPGRADE_XP_MUTIPLIER:
            upgradeXPMutiplier(1.30f);
            break;
        case UPGRADE_XP_RANGE:
            upgradeXPGrabRange(1.40f);
            break;
        case UPGRADE_POWER_UP_TIME:
            upgradePowerUpTime(1.30f);
            break;
        case UPGRADE_HEAL_AMOUNT:
            upgradeHealAmount(1.30f);
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
                SHIELD_TIME /= 1.50f;
            }
            else
            {
                SHIELD = GLOBALS::REGISTRY.create();
                GLOBALS::REGISTRY.add(SHIELD, Transform(-100.0f, -100.0f));
                GLOBALS::REGISTRY.add(SHIELD, Sprite("Shield"));
                HAS_SHEILD = true;
                // do function
            }
            break;
        case UPGRADE_ASTROIDS:
            if (HAS_ASTROID)
            {
                upgradeDamage(1.40f);
            }
            else
            {
                //angle = 0.0f;
                
                HAS_ASTROID = true;
                // do function
                 
            }
            break;
        case UPGRADE_GUNNER:
            if (HAS_GUNNER == 0)
            {
                GUNNER_L = GLOBALS::REGISTRY.create();
                GLOBALS::REGISTRY.add(GUNNER_L, Sprite("Gunner", Vector2D(16.0f, 16.0f)));
                GLOBALS::REGISTRY.add(GUNNER_L, Transform(Vector2D(100.0f, 100.0f)));
                HAS_GUNNER++;
            }
            else if (HAS_GUNNER == 1)
            {
                GUNNER_R = GLOBALS::REGISTRY.create();
                GLOBALS::REGISTRY.add(GUNNER_R, Sprite("Gunner", Vector2D(16.0f, 16.0f)));
                GLOBALS::REGISTRY.add(GUNNER_R, Transform(Vector2D(100.0f, 100.0f)));
                HAS_GUNNER++;
            }
            else
            {
                HAS_GUNNER = 2;
                upgradeDamage(1.40f);
            }
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
        case UPGRADE_POWER_UP_TIME:
            return "PowerUpTIme";
            break;
        case UPGRADE_HEAL_AMOUNT:
            return "HealAmount";
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

void PlayerSystems::playerCollisionSystem(std::vector<CollisionEvent> &collisons)
{
    
    for(const CollisionEvent& collision : collisons)
    {
        BoxCollider* a = GLOBALS::REGISTRY.get<BoxCollider>(collision.a);
        BoxCollider* b = GLOBALS::REGISTRY.get<BoxCollider>(collision.b);
        
        if (a -> tag == "player" && b -> tag == "sattelite")
        {
            AudioManager::getInstance().playSound("PowerUp");
            if (POWER_UP)
            {
                POWER_UP_START += 15000;
            }
            else
            {
                powerUp();
                deadEntities().push_back(collision.b);
            }
            continue;
        }
        else if (a -> tag == "sattelite" && b -> tag == "player")
        {
            AudioManager::getInstance().playSound("PowerUp");
            if (POWER_UP)
            {
                POWER_UP_START += 15000;
            }
            else
            {
                powerUp();
                deadEntities().push_back(collision.a);
            }
            continue;
        }
        
        if (!HAS_ASTROID)
        {
            continue;
        }
        else if (a -> tag == "Astroid" && b -> tag == "enemy")
        {
            AudioManager::getInstance().playSound("EnemyHit");
            Health* enemy_health = GLOBALS::REGISTRY.get<Health>(collision.b);
            enemy_health -> takeDamage(DAMAGE * 1.2);
            if (!enemy_health -> isAlive())
            {
                AudioManager::getInstance().playSound("EnemyDeath");
                Transform* enemy_position = GLOBALS::REGISTRY.get<Transform>(collision.b);
                deadEntities().push_back(collision.b);
                XPSystem xp;
                xp.spawnXPDrop(enemy_position -> position);
                // spawn xp pick up
            }
            continue;
            
        }
        else if (a -> tag == "enemy" && b -> tag == "Astroid")
        {
            AudioManager::getInstance().playSound("EnemyHit");

            Health* enemy_health = GLOBALS::REGISTRY.get<Health>(collision.a);
            enemy_health -> takeDamage(DAMAGE * 2);
            if (!enemy_health -> isAlive())
            {
                AudioManager::getInstance().playSound("EnemyDeath");
                Transform* enemy_position = GLOBALS::REGISTRY.get<Transform>(collision.a);
                deadEntities().push_back(collision.b);
                XPSystem xp;
                xp.spawnXPDrop(enemy_position -> position);
                // spawn xp pick up
            }
            continue;
        }
        
    }
}

void PlayerSystems::powerUp()
{
    if (POWER_UP)
    {
        switch (CURRENT_POWER_UP)
        {
            case POWER_UP_INVINCIBLE:
                GLOBALS::INVINCIBLE_CONSTANT = false;
                break;
            case POWER_UP_GRAB_XP:
                
                break;
            case POWER_UP_DOUBLE_XP:
                upgradeXPMutiplier(0.5f);
                break;
            case POWER_UP_FREE_LEVEL:
                break;
            case POWER_UP_FREEZE_TIME:
                GLOBALS::FREEZE = false;
                break;
            case POWER_UP_DOUBLE_DAMAGE:
                //SDL_Log("%f", DAMAGE);
                upgradeDamage(0.5f);
                //SDL_Log("%f", DAMAGE);
                break;
         default:
                break;
        }
        POWER_UP = false;
    }
    else
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distr(1, 6);
        int num = distr(gen);
        //int num = 5;
        switch (num)
        {
            case 1:
            {
                CURRENT_POWER_UP = POWER_UP_INVINCIBLE;
                GLOBALS::INVINCIBLE_CONSTANT = true;
            }
                break;
            case 2:
            {
                CURRENT_POWER_UP = POWER_UP_GRAB_XP;
                
            }
                break;
            case 3:
            {
                CURRENT_POWER_UP = POWER_UP_DOUBLE_XP;
                upgradeXPMutiplier(2.0f);
            }
                break;
            case 4:
            {
                CURRENT_POWER_UP = POWER_UP_FREE_LEVEL;
                XPSystem::free_level = true;
            
            }
                break;
            case 5:
            {
                CURRENT_POWER_UP = POWER_UP_FREEZE_TIME;
                GLOBALS::FREEZE = true;
            }
                break;
            case 6:
            {
                CURRENT_POWER_UP = POWER_UP_DOUBLE_DAMAGE;
                //SDL_Log("%f", DAMAGE);
                upgradeDamage(2.0f);
                //SDL_Log("%f", DAMAGE);
            }
                break;
            default:
                break;
        }
        POWER_UP_START = SDL_GetTicks();
        POWER_UP = true;
    }
}

void PlayerSystems::reset()
{
    LAST_TIME = 0.0f;
    LAST_TIME_SHIELD = 0.0f;
    LAST_TIME_GUNNER = 0.0f;
    POWER_UP_START = 0.0f;
    SPAWN_TIME = 0.0f;
    RATE_OF_FIRE = 1000.0f;
    DAMAGE = 10.0f;
    PROJECTILE_SPEED = 250.0f;
    SHIELD_TIME = 5000.0f;
    HAS_SHEILD = false;
    HAS_ASTROID = false;
    POWER_UP = false;
    angle = 0.0f;
    HAS_GUNNER = 0;
    CURRENT_WEAPON = WEAPON_NORMAL;
    CURRENT_POWER_UP = POWER_UP_NONE;
}
