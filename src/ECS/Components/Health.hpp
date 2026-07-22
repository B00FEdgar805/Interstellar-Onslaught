#ifndef Health_hpp
#define Health_hpp

#include "Component.hpp"

class Health final : public BaseComponent
{
private:
    float TOTAL_HEALTH = 0.0f;
    float HEALTH = 0.0f;
public:
    Health() = default;
    Health(float health)
    :   TOTAL_HEALTH(health),
        HEALTH(TOTAL_HEALTH)
    {}
    
    
    void takeDamage(float damage)
    {
        HEALTH -= damage;
    }
    
    bool isAlive()
    {
        return HEALTH > 0.0f;
    }
    
    void heal(float value)
    {
        if(HEALTH + value < TOTAL_HEALTH)
        {
            HEALTH += value;
        }
        else
        {
            HEALTH = TOTAL_HEALTH;
        }
    }
    
    void upgradeHealth(float value)
    {
        TOTAL_HEALTH *= value;
    }
};


#endif /* Health_hpp */
