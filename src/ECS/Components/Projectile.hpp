#ifndef Projectile_hpp
#define Projectile_hpp

#include "Component.hpp"
#include "../Types.hpp"

class Projectile final : public BaseComponent
{
private:
    float RANGE;
    float SPEED;
    float DISTANCE = 0;
    float DAMAGE;
    float RATE_OF_FIRE;
    
public:
    Entity OWNER;

    Projectile() = default;
    Projectile(int range, float speed, float RoF, float damage, Entity owner)
    :   RANGE(range),
        SPEED(speed),
        RATE_OF_FIRE(RoF),
        DAMAGE(damage),
        OWNER(owner)
    {}
    
    void addDistance(float delta_time)
    {
        DISTANCE += (delta_time * SPEED);
        //std::cout << DISTANCE << std::endl;
    }
    
    bool hasExpired()
    {
        return DISTANCE >= RANGE;
    }
    
    float getDamage()
    {
        return DAMAGE;
    }
    
    
};

#endif /* Projectile_hpp */
