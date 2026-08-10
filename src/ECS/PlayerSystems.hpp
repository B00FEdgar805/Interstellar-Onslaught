//
//  PlayerSystems.hpp
//  GameTestSDL3
//
//  Created by Edgar Alamillo on 7/8/26.
//

#ifndef PlayerSystems_hpp
#define PlayerSystems_hpp

#include "Types.hpp"
#include "Registry.hpp"
#include "ProjectileSystem.hpp"
#include "SDL3/SDL.h"

class PlayerSystems
{
private:
    Entity PLAYER;
    Entity ASTROID_L;
    Entity ASTROID_R;
    Entity GUNNER_L;
    Entity GUNNER_R;
    Entity SHIELD;
    Uint64 LAST_TIME = 0.0f;
    Uint64 LAST_TIME_SHIELD = 0.0f;
    Uint64 LAST_TIME_GUNNER = 0.0f;
    float RATE_OF_FIRE = 1000.0f;
    float DAMAGE = 5.0f;
    float PROJECTILE_SPEED = 250.0f;
    float SHIELD_TIME = 5000.0f;
    //int MAX = 8;
    //inline static int UNIQUE_UPGRADE = 0;
    bool HAS_SHEILD = false;
    bool HAS_ASTROID = false;
    float angle = 0.0f;
    int HAS_GUNNER = 0;
    

    enum WeaponType
    {
        WEAPON_NORMAL,
        WEAPON_SHOTGUN,
        WEAPON_SMG,
        WEAPON_RAILGUN
    };
    
    WeaponType CURRENT_WEAPON = WEAPON_NORMAL;

    //Registry REGISTRY;
public:
    
    PlayerSystems(Entity player);
    void fireSystem(ProjectileSystem projectiles, float delta);
    bool shoot(float time);
    void upgradeROF(float value);
    void upgradeHealth(float value);
    void upgradeDamage(float value);
    void upgradeSpeed(float value);
    void upgradeProjectileSpeed(float value);
    void upgradeXPMutiplier(float value);
    void upgradeXPGrabRange(float value);
    
    enum UpgradeType
    {
        UPGRADE_ROF,
        UPGRADE_HEALTH,
        UPGRADE_DAMAGE,
        UPGRADE_SPEED,
        UPGRADE_PROJECTILE_SPEED,
        UPGRADE_XP_MUTIPLIER,
        UPGRADE_XP_RANGE,
        UPGRADE_WEAPON_NORMAL,
        UPGRADE_WEAPON_SHOTGUN,
        UPGRADE_WEAPON_SMG,
        UPGRADE_WEAPON_RAILGUN,
        UPGRADE_SHEILD,
        UPGRADE_GUNNER,
        UPGRADE_ASTROIDS
        
    };
    
    inline static UpgradeType left;
    inline static UpgradeType middle;
    inline static UpgradeType right;
    
    
    
    void upgrade(UpgradeType button);
    UpgradeType randomUpgrade();
    UpgradeType randomUpgradeUnique();
    UpgradeType randomUpgradeWeapon();
    static std::string getLabel(UpgradeType button);

};
#endif /* PlayerSystems_hpp */

