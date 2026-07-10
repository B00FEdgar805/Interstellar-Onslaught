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
    Uint64 LAST_TIME = 0.0f;
    float RATE_OF_FIRE = 1000.0f;
    float DAMAGE = 10.0f;

    //Registry REGISTRY;
public:
    
    PlayerSystems(Entity player);
    void fireSystem(ProjectileSystem projectiles, Registry &REGISTRY);
    bool shoot(float time);
    void upgradeROF(float value);
    void upgradeHealth(float value, Registry &REGISTRY);
    void upgradeDamage(float value);
    void upgradeSpeed(float value, Registry &REGISTRY);
};
#endif /* PlayerSystems_hpp */
