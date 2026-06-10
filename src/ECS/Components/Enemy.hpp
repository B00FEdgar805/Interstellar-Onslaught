//
//  Enemy.hpp
//  GameTestSDL3
//
//  Created by Edgar Alamillo on 6/5/26.
//

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
