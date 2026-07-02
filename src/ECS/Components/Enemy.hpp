#ifndef Enemy_hpp
#define Enemy_hpp

#include "Component.hpp"

class Enemy final : public BaseComponent
{
    float DAMAGE = 0.0f;
public:
    Enemy() = default;
    Enemy(float damage)
    :   DAMAGE(damage)
    {}
    
    float getDamage()
    {
        return DAMAGE;
    }
};

#endif /* Enemy_hpp */
