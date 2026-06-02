#ifndef Velocity_hpp
#define Velocity_hpp

#include "Component.hpp"

class Velocity final : public BaseComponent
{
public:
    float m_speed;
    Vector2D value;
    
    Velocity() = default;
    
    Velocity(float x, float y, float speed)
    {
        value.x = x;
        value.y = y;
        m_speed = speed;
    }
    
    Velocity(const Vector2D& v, float speed)
    {
        value = v;
        m_speed = speed;
    }
    
    Velocity(float speed)
    {
        m_speed = speed;
    }
};
#endif /* Velocity_hpp */
