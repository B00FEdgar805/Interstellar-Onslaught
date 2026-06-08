#ifndef Velocity_hpp
#define Velocity_hpp

#include "Component.hpp"

class Velocity final : public BaseComponent
{
public:
    float m_speed;
    Vector2D value;
    Vector2D direction;
    
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
    
    float directionToDegrees()
    {
        double radians = std::atan2(direction.x, direction.y);
        double degrees = radians * (180 / M_PI);
        if(degrees < 0)
        {
            degrees += 360;
        }
        return -(degrees - 180);
    }
};
#endif /* Velocity_hpp */
