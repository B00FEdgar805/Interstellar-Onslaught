#ifndef PlayerSystems_hpp
#define PlayerSystems_hpp

#include "Types.hpp"
#include "Registry.hpp"
#include "ProjectileSystem.hpp"
#include "SDL3/SDL.h"
#include "CollisionSystem.hpp"
#include "DamageNumberSystem.hpp"


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
    Uint64 POWER_UP_START = 0.0f;
    float SPAWN_TIME = 0.0f;
    float RATE_OF_FIRE = 1000.0f;
    float DAMAGE = 10.0f;
    float PROJECTILE_SPEED = 250.0f;
    float SHIELD_TIME = 5000.0f;
    bool HAS_SHEILD = false;
    bool HAS_ASTROID = false;
    int POWER_UP_TIME = 15000;
    float HEAL_AMOUNT = 50.0f;
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

    enum PowerUp
    {
        POWER_UP_NONE,
        POWER_UP_INVINCIBLE,
        POWER_UP_GRAB_XP,
        POWER_UP_FREE_LEVEL,
        POWER_UP_DOUBLE_DAMAGE,
        POWER_UP_DOUBLE_XP,
        POWER_UP_FREEZE_TIME,
        POWER_UP_HEAL

    };
    
    static inline PowerUp CURRENT_POWER_UP = POWER_UP_NONE;
    
    //Registry REGISTRY;
public:
    static inline bool POWER_UP = false;

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
    void upgradePowerUpTime(float value);
    void upgradeHealAmount(float value);
    void playerCollisionSystem(std::vector<CollisionEvent>& collisons, DamageNumberSystem damage_num);
    
    enum UpgradeType
    {
        UPGRADE_ROF,
        UPGRADE_HEALTH,
        UPGRADE_DAMAGE,
        UPGRADE_SPEED,
        UPGRADE_PROJECTILE_SPEED,
        UPGRADE_XP_MUTIPLIER,
        UPGRADE_XP_RANGE,
        UPGRADE_POWER_UP_TIME,
        UPGRADE_HEAL_AMOUNT,
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
    static std::string getLabelPowerUp();
    //bool powerUpActive();
    void powerUp();
    void reset();
    

};
#endif /* PlayerSystems_hpp */

