#ifndef Health_hpp
#define Health_hpp

#include "Component.hpp"

class Health final : public BaseComponent
{
private:
    float HEALTH = 0.0f;
    
public:
    Health() = default;
    Health(float health)
    :   HEALTH(health)
    {}
    void takeDamage(float damage)
    {
        HEALTH -= damage;
    }
    
    bool isAlive()
    {
        return HEALTH > 0.0f;
    }
}


#endif /* Health_hpp */
